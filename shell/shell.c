//a unix-style shell using pre-built functions in jobs.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include "./jobs.h"

job_list_t *jobby;
int job_count = 1;
void reap();
void handle(char *buffer);
void execute_commands(char *full_path, char **argv, char *input, char *output, int append_flag, int bg_flag);
int main() {
	// want to ignore any of these calls in the shell
	if(signal(SIGINT, SIG_IGN) == SIG_ERR) {
		perror("signal");
	}
	if(signal(SIGTSTP, SIG_IGN) == SIG_ERR) {
		perror("signal");
	}
	if(signal(SIGQUIT, SIG_IGN) == SIG_ERR) {
		perror("signal");
	}
	if(signal(SIGTTOU, SIG_IGN) == SIG_ERR) {
		perror("signal");
	}
	jobby = init_job_list();

    char buf[1024]; 
    #ifdef PROMPT
	if (printf("$ ") < 0) {
		perror("printf");
	}
	fflush(stdout);
	#endif
	int ret;
	while((ret = (int) read(STDIN_FILENO, buf, 1024)) != 0) {
		
		buf[ret-1] = '\0';

		if(buf[0] == '\0') {
			// if input was just \n
			#ifdef PROMPT
			if (printf("$ ") < 0) {
				perror("printf");
			}
			fflush(stdout);
			#endif
			reap(); 
			continue;
		}
		
		if(ret < 0) {
			printf("%s\n", buf);
			perror("read");
			#ifdef PROMPT
			if (printf("$ ") < 0) {
				perror("printf");
			}
			fflush(stdout);
			#endif
			reap();
			continue;
		}
		
		handle(buf);
		reap(); 

		#ifdef PROMPT
		if (printf("$ ") < 0) {
			perror("printf");
		}
		fflush(stdout);
		#endif
	}
	cleanup_job_list(jobby);
	exit(0);
}

// handle parses the buffer, sets any input and output files
// and finally calls execute_commands with appropriate variables
void handle(char *buffer) {
	///////////////////////////////////////////////////
	// first, parse!
	char *parsed[1024];
	char *full_path;
	char *input;
	input = NULL;
	char *output;
	output = NULL;
	int count_in = 0;
	int count_out = 0;
	int parsed_count = 0;
	int append_flag = 0;  // if stays 0, not appending
	char *token = strtok(buffer, " \t");
	int bg_flag = 0;

	while(token != NULL) {
		if(strcmp(token, "<") == 0) {
			input = strtok(NULL, " \t");
			if(input == NULL) {
				fprintf(stderr, "Error: Need input for <\n");
				return;
			}
			else if(strcmp(input,">>") == 0 || strcmp(input,"<") == 0 || strcmp(input,">") == 0){
				fprintf(stderr, "Error: Input file is a redirection symbol\n");
				return;
			}
			count_in++;
		}
		else if (strcmp(token, ">") == 0) {
			output = strtok(NULL, " \t");
			if(output == NULL) {
				fprintf(stderr, "Error: Need output for >\n");
				return;
			}
			else if (strcmp(output,">>") == 0 || strcmp(output,"<") == 0 || strcmp(output,">") == 0){
				fprintf(stderr, "Error: Output file is a redirection symbol\n");
				return;
			}
			count_out++;
		}
		else if (strcmp(token, ">>") == 0) {
			append_flag = 1;
			output = strtok(NULL, " \t");
			if(output == NULL || strcmp(output,">>") == 0 || strcmp(output,"<") == 0 || strcmp(output,">") == 0) {
				fprintf(stderr, "Error: Need output for >>\n");
				return;
			}
			count_out++;
		}
		else {
			parsed[parsed_count] = token;
			parsed_count++;
		}
		if(count_in > 1 || count_out > 1) {
			fprintf(stderr, "Error: More than one redirector!\n");
			return;
		}
		token = strtok(NULL, " \t");
	}

	// finally, add to full_path variable
	full_path = parsed[0];
	if(full_path == NULL && (output != NULL || input != NULL)) {
		fprintf(stderr, "error: redirects with no command\n");
		return;
	}
	//check for & at end of token
	if(parsed_count!=0){
		if(*parsed[parsed_count-1]=='&'){
			bg_flag = 1;
			parsed[parsed_count-1]='\0';
		}
	}
	if(strcmp(parsed[0], "cd") == 0) {
		// change directory
		// check if there is parsed
		if(chdir(parsed[1]) < 0) {
			fprintf(stderr, "cd: syntax error\n");
			return;
		}
	}
	else if (strcmp(parsed[0], "ln") == 0) {
		// make hard link
		if(link(parsed[1], parsed[2]) < 0) {
			fprintf(stderr, "ln: syntax error\n");
			return;
		}
	}
	else if (strcmp(parsed[0], "jobs") == 0) { //lists all jobs
		// list all jobs
		jobs(jobby);
		return;
	}
	else if (strcmp(parsed[0], "rm") == 0) {
		// remove file
		if(unlink(parsed[1]) < 0) {
			fprintf(stderr, "rm: syntax error\n"); 
			return;
		}
	}
	else if(strcmp(parsed[0], "exit") == 0) {
		cleanup_job_list(jobby);
		exit(0);
	}
	else if(strcmp(parsed[0], "fg") == 0) {
		//send a sigcont to stopped job
		if(parsed[1] == '\0'){
			fprintf(stderr, "fg: syntax error\n");
			return;
		}
		char *jid = parsed[1];
		if(jid[0] != '%'){
			fprintf(stderr, "fg: job input does not begin with %%\n");
			return;
		}
		jid++; //want to point past %
		pid_t pid = get_job_pid(jobby, atoi(jid));
		int j_id = get_job_jid(jobby, pid);

		if(kill(-pid, SIGCONT) < 0) {
			fprintf(stderr, "job not found\n");
			return;
		}
		if(tcsetpgrp(STDIN_FILENO, pid)<0) {
			perror("tcsetpgrp");
		}
		//now have to wait for foreground process to finish and update jobs list depending on signals
		int status;
		pid_t ret;
		if((ret = waitpid(pid, &status, WUNTRACED)) < 0) { //if no error, ret is process id of terminated child
			perror("waitpid");
		}
		if(WIFSIGNALED(status)) {
			// true if terminated by a signal
			printf("[%d] (%d) terminated by signal %d\n", j_id, ret, WTERMSIG(status));
			fflush(stdout);
			if(remove_job_pid(jobby, ret) == -1){
				fprintf(stderr, "unable to remove job\n");
			}
		}
		if(WIFSTOPPED(status)) {
			// true if stopped by a signal
			printf("[%d] (%d) suspended by signal %d\n", j_id, ret, WSTOPSIG(status));
			fflush(stdout);
			if(update_job_pid(jobby, pid, _STATE_STOPPED) == -1){
				fprintf(stderr, "unable to remove job\n");
			}
		}
		if(WIFEXITED(status) == 1){
			if(remove_job_pid(jobby, ret) == -1) {
				fprintf(stderr, "unable to remove job\n");
			}
		}
		if(tcsetpgrp(STDIN_FILENO, getpgid(0)) < 0) {
			perror("tcsetpgrp");
		}
		return;
	}	

	else if(strcmp(parsed[0], "bg") == 0) {
		//send a sigcont to stopped job
		if(parsed[1] == '\0') {
			fprintf(stderr, "bg: syntax error\n");
			return;
		}		
		char *jid = parsed[1];
		if(jid[0]!='%'){
			fprintf(stderr, "bg: job input does not begin with %%\n");
			return;
		}
		jid++;
		int j_id = atoi(jid); //want to point past %
		pid_t pid = get_job_pid(jobby, j_id);
		if(kill(-pid, SIGCONT) < 0) {
			fprintf(stderr, "job not found\n");
			return;
		}
	}
	// if not, execute our commands
	else {
		char *temp;
		temp = strrchr(parsed[0], '/');  // returns portion of haystack starting at last occurrence of /
		// don't need to error check for this one
		if(temp != NULL) {
			temp++;  //gets rid of /
			parsed[0] = temp;
		}
		parsed[parsed_count] = NULL;
		execute_commands(full_path, parsed, input, output, append_flag, bg_flag);
	}
	return;
}

// execute_commands deals with input/output redirection and calls execv
// execv is called with the full file path and the final command in the path (both passed in)
void execute_commands(char *full_path, char **argv, char *input, char *output, int append_flag, int bg_flag) {
	int res = fork();
	if (res == 0) { // in child process now
		//set group ids
		if(setpgid(0, 0) < 0) { //sets group of this process to itself
			perror("setpgid"); 
		}
		if(bg_flag == 0) { //in foreground
			if(tcsetpgrp(STDIN_FILENO, getpgid(0))<0){ // sets group to foreground
				perror("tcsetpgrp");
			}
		}

		// now, check if we have redirection
		if(output != NULL) {
			if(close(STDOUT_FILENO) < 0) {
				perror("close");
				exit(1);
			}
			else {
				if(append_flag == 0) {
					// truncate
					int flags = O_WRONLY | O_TRUNC | O_CREAT;
					// 0600 is read and write permissions to yourself, no one else
					if(open(output, flags, 0600) < 0) {
						perror("open");
						exit(1);
					}
				}
				else {
					// append
					int flags2 = O_WRONLY | O_APPEND | O_CREAT;
					if(open(output, flags2, 0600) < 0) {
						perror("open");
						exit(1);
					}
				}
			}
		}
		if(input != NULL) {
			if(close(STDIN_FILENO) < 0) {
				perror("close");
				exit(1);
			}
			else {
				int flags3 = O_RDONLY;
				if(open(input, flags3) < 0) {
					perror("open");
					exit(1);
				}
			}
		}		
		//set signals to default so child will now accept these signals
		if(signal(SIGINT, SIG_DFL) == SIG_ERR) {
			perror("signal");
		}
		if(signal(SIGTSTP, SIG_DFL) == SIG_ERR) {
			perror("signal");
		}
		if(signal(SIGQUIT, SIG_DFL) == SIG_ERR) {
			perror("signal");
		}
		if(signal(SIGTTOU, SIG_DFL) == SIG_ERR) {
			perror("signal");
		}
		execv(full_path, argv);
		perror("execv");
		exit(1);
	}

	else if (res > 0) { // parent process, aka shell
		// here, res IS the pid of the child
		if(bg_flag == 0) { // in foreground! means we're in the child process
			int status;
	 		if(waitpid(res, &status, WUNTRACED) < 0) { //waits for foreground job to finish
	 			perror("waitpid");
	 		}
			if(WIFSIGNALED(status)) {
				// true if terminated by a signal
				printf("[%d] (%d) terminated by signal %d\n", job_count, res, WTERMSIG(status));
				fflush(stdout);
			}
			if(WIFSTOPPED(status)) {
				// true if stopped by a signal
				add_job(jobby, job_count, res, _STATE_STOPPED, full_path);
				printf("[%d] (%d) suspended by signal %d\n", job_count, res, WSTOPSIG(status));
				job_count++;
				fflush(stdout);
			}
		}
		//now add to job list if background
		else if(bg_flag==1) {
			add_job(jobby, job_count, res, _STATE_RUNNING, full_path);
			printf("[%d] (%d)\n", job_count, res); //jid, pid
			fflush(stdout);
			job_count++;
		}
	
	} // automatically returns to while loop
	else {
		// if fork returns a negative number, aka Operating System cannot create fork, return error
		perror("fork");
	}
	if(tcsetpgrp(STDIN_FILENO, getpgid(0)) < 0) { // sets group to foreground
		perror("tcsetpgrp");
	}
}

// reap loops through our job list and updates it and any jobs if signals were received
void reap(){
	pid_t pid;
	while((pid = get_next_pid(jobby)) != -1) {
		int status;
		int ret;
		if((ret = waitpid(pid, &status, WNOHANG | WUNTRACED | WCONTINUED)) < 0) {
			perror("waitpid");
		}
		else if(ret > 0) {
			if(WIFEXITED(status)) {
				// true if terminated normally
				printf("[%d] (%d) terminated with exit status %d\n", get_job_jid(jobby, pid), pid, WEXITSTATUS(status));
				fflush(stdout);
				if(remove_job_pid(jobby, pid) == -1) {
					fprintf(stderr, "unable to remove job\n");
				}
			}
			if(WIFSIGNALED(status)) {
				// true if terminated by a signal
				printf("[%d] (%d) terminated by signal %d\n", get_job_jid(jobby, pid), pid, WTERMSIG(status));
				fflush(stdout);
				if(remove_job_pid(jobby, pid) == -1) {
					fprintf(stderr, "unable to remove job\n");
				}
			}
			if(WIFSTOPPED(status)) {
				// true if stopped by a signal
				printf("[%d] (%d) suspended by signal %d\n", get_job_jid(jobby, pid), pid, WSTOPSIG(status));
				fflush(stdout);
				if(update_job_jid(jobby, get_job_jid(jobby, pid), _STATE_STOPPED) == -1){
					fprintf(stderr, "unable to update job\n");
				}
			}
			if(WIFCONTINUED(status)) {
				// true if resumed by sigcont
				printf("[%d] (%d) resumed\n", get_job_jid(jobby, pid), pid);
				fflush(stdout);
				if(update_job_jid(jobby, get_job_jid(jobby, pid), _STATE_RUNNING) == -1){
					fprintf(stderr, "unable to update job\n");
				}
			}
		}
	}
}
