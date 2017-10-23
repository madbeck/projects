#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"

//RANDOM MAZE GENERATOR
/*
 * Given a direction, get its opposite
 *
 * Parameters:
 *	- dir: direction
 *
 * Returns:
 *	- the opposite direction to dir
 */
Direction get_opposite_dir(Direction dir) {
	//Fill in this function
	if(dir == NORTH){
		return SOUTH;
	}
	else if(dir == SOUTH){
		return NORTH;
	}
	else if(dir == EAST){
		return WEST;
	}
	else{
		return EAST;
	}
}


/*
 * Given an array of the four directions, randomizes the order of the directions
 *
 * Parameters:
 *	- directions: an array of Direction enums of size 4
 *
 * Returns:
 *	- nothing - the array should be shuffled in place
 */
void shuffle_array(Direction directions[]) {
	//SHUFFLES the directions
	for(int i = 0; i < 4; i++){
		Direction temp_i;
		int rand_num = ((rand()%(4-i)) + i);
		temp_i = directions[i];
		directions[i] = directions[rand_num];
		directions[rand_num] = temp_i;
	}
}

/*
 * Recursive function for stepping through a maze and setting its connections/walls using
 * the drunken_walk algorithm
*/
void drunken_walk(int row, int col, int num_rows, int num_cols, struct maze_room maze[num_rows][num_cols]) {
    //Fill in this function CALLS shuffle array
    struct maze_room *r;
    r = &maze[row][col];
    r->visited = 1;
    Direction directions[] = {0,1,2,3};    
    shuffle_array(directions);
  
	for(int i = 0; i < 4; i++){
    		if(directions[i] == NORTH && is_in_range(row-1, col, num_rows, num_cols)==0){
    			r->connections[0] = 1; //is_in_range returns 0 if it's not in range
    		}
    		else if(directions[i] == EAST && is_in_range(row, col+1, num_rows, num_cols)==0){
    			r->connections[3] = 1;
    		}
    		else if(directions[i] == SOUTH && is_in_range(row+1, col, num_rows, num_cols)==0){
    			r->connections[1]= 1;
    		}
    		else if(directions[i] == WEST && is_in_range(row, col-1, num_rows, num_cols)==0){
    			r->connections[2] = 1;
    		}
    	
    	else{ //if in range
    		//call get neighbor NOW because we know the neighbor is in range
    		struct maze_room *neighbor = get_neighbor(num_rows, num_cols, maze, r, directions[i]);
    		if(neighbor->visited == 0){ //aka neighbor has not been visited
	    		r->connections[directions[i]] = 0;
	    		drunken_walk(neighbor->row, neighbor->col, num_rows, num_cols, maze); //not heighbor.row, neighbor.col?
    		} //neighbor HAS been visited
    		else{
    			int opposite_dir = get_opposite_dir(directions[i]);
    			if(neighbor->connections[opposite_dir] == 1 || neighbor->connections[opposite_dir] == 0){
    				r->connections[directions[i]]= neighbor->connections[opposite_dir];
    			}
    			else{
    				r->connections[directions[i]] = 1;
    			}
    		}
    	}
    }
}

/*
 * Represents a maze_room struct as a hexadecimal number based on its connections
 *
 * Parameters:
 *	- room: a struct maze_room to be converted to a hexadecimal number
 *
 * Returns:
 *	- the hexadecimal representation of room
 */
int encode_room(struct maze_room room) {
	//Fill in this function
	int hexadecimal;
	hexadecimal = 8*room.connections[3] + 4*room.connections[2] + 2*room.connections[1] + 1*room.connections[0];
	return hexadecimal;
}

/*
 * Represents a maze_room array as a hexadecimal array based on its connections
 *
 * Parameters:
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *	- maze: a 2D array of maze_room structs representing the maze
 *	- result: a 2D array of hexadecimal numbers representing the maze
 *
 * Returns:
 *	- nothing - each maze_room in the maze should be represented
 *	  as a hexadecimal number  and put in nums at the corresponding index
 */
void encode_maze(int num_rows, int num_cols, struct maze_room maze[num_rows][num_cols], int result[num_rows][num_cols]) {
	//Fill in this function=
	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j++){
			int hdecimal = encode_room(maze[i][j]);
			result[i][j] = hdecimal;
		}
	}
}

/*
 * Main function
 *	
 * Parameters:
 *	- argc: the number of command line arguments - for this function 4
 *	- **argv: a pointer to the first element in the command line
 *            arguments array - for this function:
 *		      ["generator", <output file>, <number of rows>, <number of columns>]
 *
 * Returns:
 *	- 0 if program exits correctly, 1 if there is an error
 */
int main(int argc, char **argv) {
	srand(time(NULL));
	char *file_name;
	int num_rows;
	int num_cols;

	if (argc != 4) {
		printf("Incorrect number of arguments.\n");
		printf("./generator <output file> <number of rows> <number of columns>\n");
		return 1;
	} else {
		file_name = argv[1];
		num_rows = atoi(argv[2]);
		num_cols = atoi(argv[3]);
	}

	//following taken from life.c
	if (num_rows <= 0 || num_cols <= 0) {
    fprintf(stderr, "Dimension must be positive.\n");
    return 1;
  	}

	FILE *in_file = fopen(file_name, "w");
 	if (in_file == NULL) {
    fprintf(stderr, "Could not open file.\n");
    return 1;
  	}

	//Fill in this function
	//initialize the array of structures and assign properties defined in common.h to each
	struct maze_room maze[num_rows][num_cols];
	for(int m = 0; m < num_rows; m++){
		for(int n = 0; n < num_cols; n++){
			struct maze_room room;
			room.row = m;
			room.col = n;
			room.visited = 0;
			room.connections[0] = 2;
			room.connections[1] = 2;
			room.connections[2] = 2;
			room.connections[3] = 2;
			maze[m][n] = room;
		}
	}

	int result[num_rows][num_cols]; //this will eventually hold the rooms in hex-decimal notation
	int row = 0;
	int col = 0;
	
	drunken_walk(row, col, num_rows, num_cols, maze);
	encode_maze(num_rows, num_cols, maze, result);

	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j++){
			if(fprintf(in_file, "%x", result[i][j]) < 0){
    			fprintf(stderr, "Nothing to print\n");
				exit(1);
			}
		}
		fprintf(in_file, "\n");

	}
  	if(fclose(in_file)) {
    	fprintf(stderr, "Could not close the file\n");
    	return 1;
  	}
	return 0;
}

//SOLVER
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "solver.h"


/*
 * Given a pointer to a maze_room, set its connections in all four directions based
 * on the hex value passed in.
 *
 * For example:
 *
 * 		create_room_connections(&maze[3][4], 0xb)
 *
 * 0xb is 1011 in binary, which means we have walls on all sides except the WEST.
 * This means that every connection in maze[3][4] should be set to 1, except the WEST
 * connection, which should be set to 0.
 *
 * See the handout for more details about our hexadecimal representation, as well as
 * examples on how to use bit masks to extract information from this hexadecimal
 * representation.
 *
 * Parameters:
 *	- room: pointer to the current room
 *	- hex: hexadecimal integer (between 0-15 inclusive) that represents the connections
 *		   in all four directions from the given room.
 *
 * Returns:
 *	- nothing. The connections should be saved in the maze_room struct pointed to by
 *	  the parameter (make sure to use pointers correctly!).
 */
void create_room_connections(struct maze_room *room, unsigned int hex) {
	//Fill in this function
	room->connections[0] = (hex & 0x1); //want every value at zero except for 1st
	room->connections[1] = (hex & 0x2)/2;
	room->connections[2] = (hex & 0x4)/4;
	room->connections[3] = (hex & 0x8)/8;
}



/*
 * Recursive depth-first search algorithm for solving random mazes.
 * This function should also print out either every visited room as it goes, or
 * the final pruned solution, depending on whether the FULL macro is set.
 * Parameters:
 *	- row: row of the current room
 *	- col: column of the current room
 *	- goal_row: row of the goal room
 *	- goal_col: col of the goal room
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *	- maze: a 2d array of maze_room structs representing your maze
 *	- file: the file to write the solution to
 * Returns:
 *	- 1 if the current branch finds a valid solution, 0 if no branches are valid.
 */
int dfs(int row, int col, int goal_row, int goal_col, int num_rows, int num_cols,
		struct maze_room maze[num_rows][num_cols], FILE *file) {
	//Fill in this function
	
	#ifdef FULL
	if(fprintf(file, "%d, %d\n", row, col) < 0){
    	fprintf(stderr, "Unable to print\n");
		exit(1);
	}
	#endif

	if(row==goal_row && col==goal_col){
		return 1;
	}
	struct maze_room *room=&maze[row][col];
	room->visited = 1;

	for(int dir = 0; dir < 4; dir++){
		struct maze_room *neighbor = get_neighbor(num_rows, num_cols, maze, room, dir);
		//struct maze_room *neighbor = get_neighbor(num_rows, num_cols, maze, room, dir);
		if(neighbor == NULL){
			continue; //go back to the for loop
		}

		if(room->connections[dir]==0 && neighbor->visited==0){
			if(dfs(neighbor->row, neighbor->col, goal_row, goal_col, num_rows, num_cols, maze, file)==1){
				room->next=neighbor; //IMPORTANt
				return 1;
			}
			else{
				#ifdef FULL
				if(fprintf(file, "%d, %d\n", row, col) < 0){
					fprintf(stderr, "Nothing to print\n");
					exit(1);
				}
				#endif
			}
		}
	}
	return 0;
}

/*
 * Main function
 *
 * Parameters:
 *	- argc: the number of command line arguments - for this function 9
 *	- **argv: a pointer to the first element in the command line
 *            arguments array - for this function:
 *		      ["solver", <input maze file>, <number of rows>, <number of columns>
 *				 <output path file>, <starting row>, <starting column>, 
 *				 <ending row>, <ending column>]
 *
 * Returns:
 *	- 0 if program exits correctly, 1 if there is an error
 */

int main(int argc, char **argv) {
	int num_rows, num_cols, start_row, start_col, goal_row, goal_col;
	char *maze_file;
	char *path_file;
	if (argc != 9) {
		printf("Incorrect number of arguments.\n");
		printf("./solver <input maze file> <number of rows> <number of columns>");
		printf(" <output path file> <starting row> <starting column>");
		printf(" <ending row> <ending column>\n");
		return 1;
	} else {
		maze_file = argv[1];
		num_rows = atoi(argv[2]);
		num_cols = atoi(argv[3]);
		path_file = argv[4];
		start_row = atoi(argv[5]);
		start_col = atoi(argv[6]);
		goal_row = atoi(argv[7]);
		goal_col = atoi(argv[8]);
	}
	//Fill in this function
	FILE *in_file = fopen(maze_file, "r");
	if (in_file == NULL){
		fprintf(stderr, "Could not open file.\n");
		return 1;
	}

	FILE *out_file = fopen(path_file, "w");
	if (out_file == NULL){
		fprintf(stderr, "Could not open file.\n");
		return 1;
	}

	struct maze_room maze[num_rows][num_cols];
	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j++){
			struct maze_room *room = &maze[i][j];
			room->next = NULL;
			room->connections[0] = 2;
			room->connections[1] = 2;
			room->connections[2] = 2;
			room->connections[3] = 2;
			room->visited = 0;
			room->row = i;
			room->col = j;
			unsigned int hex_num = 0U;
			if(fscanf(in_file, "%1x", &hex_num) < 0) {
    			fprintf(stderr, "Nothing to print\n");
    			return 1;
  			}
			create_room_connections(room, hex_num);
		}
	}

  	if(fclose(in_file)==EOF) {
    	fprintf(stderr, "Could not close the file\n");
    	return 1;
  	}

	#ifdef FULL
	fprintf(out_file, "FULL\n");
	#endif
	#ifndef FULL
	fprintf(out_file, "PRUNED\n");
	#endif

	dfs(start_row, start_col, goal_row, goal_col, num_rows, num_cols, maze, out_file);

	#ifndef FULL //if not full, print the pruned part
	struct maze_room *room = &maze[start_row][start_col];
	while(room->next!=NULL){
		if(fprintf(out_file, "%d, %d\n", room->row, room->col) < 0){
    		fprintf(stderr, "Nothing to print\n");
			exit(1);
		}
		room = room->next;
	}
	if(fprintf(out_file, "%d, %d\n", room->row, room->col) < 0){
    	fprintf(stderr, "Nothing to print\n");
		return 1;
	}
	#endif

  	if(fclose(out_file)==EOF) {
    	fprintf(stderr, "Could not close the file\n");
    	return 1;
  	}
	return 0;
}

//ROOM VALIDATOR
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

/*
 * Determines whether or not the room at [row][col] is a valid room within the maze with
 * dimensions num_rows x num_cols
 *
 * Parameters:
 *	- row: row of the current room
 *	- col: column of the current room
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 * 
 * Returns:
 *	- 0 if room is not in the maze, 1 if room is in the maze
 */
int is_in_range(int row, int col, int num_rows, int num_cols) {
	//Fill in this function
	if(((0 <= row) && (row < num_rows)) && ((0 <= col) && (col < num_cols))){
		return 1; //1 if IN RANGE
	}
	else{
		return 0;
	}
}

/*
 * Given a pointer to the room and a Direction to travel in, return a pointer to the room
 * that neighbors the current room on the given Direction.
 * For example: get_neighbor(&maze[3][4], EAST) should return &maze[3][5]
 *
 * Parameters:
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *	- room: pointer to the current room
 *	- dir: Direction to get the neighbor from
 *	- maze: a 2D array of maze_room structs representing your maze
* Returns:
 *	- pointer to the neighboring room
 */
 //get_neighbour is returning a pointer to a structure maze_room
struct maze_room *get_neighbor(int num_rows, int num_cols, struct maze_room maze[num_rows][num_cols],
			                    struct maze_room *room, Direction dir) {
	//Fill in this function
	
	if(dir == 0){
		if(is_in_range(room->row-1, room->col, num_rows, num_cols)==1){
			return &maze[room -> row-1][room -> col];
		}
	}
	if(dir == 1){
		if(is_in_range(room->row+1, room->col, num_rows, num_cols)==1){
			return &maze[room -> row+1][room -> col];
		}
	}
	if(dir == 2){
		if(is_in_range(room->row, room->col-1, num_rows, num_cols)==1){
			return &maze[room -> row][room -> col-1];
		}
	}
	if(dir == 3){
		if(is_in_range(room->row, room->col+1, num_rows, num_cols)==1){
			return &maze[room -> row][room -> col+1];
		}
	}
	return NULL;
}


//FINAL FILE, holds maze struct and directions
/*
 * Enum to represent the four directions
 * Here is an example of how to use an enum:
 *
 * Direction dir = EAST;
 * if (dir == EAST) {
 *     printf("Heading EAST!");
 * }
 */
/*
#include <stdbool.h>

typedef enum {
	NORTH = 0,
	SOUTH = 1,
	WEST = 2,
	EAST = 3
} Direction;


 //Struct to represent a room in the maze.
 //Must store:

struct maze_room {
  struct maze_room *next;
  // Fill in the rest of the fields you need
  // int maze[num_rows][num_cols]; ?? necessary?
  int row;
  int col;
  int visited; // 1 if visited, 0 if not visited
  int connections[4]; //0=north, 1=south, 2=west, 3=east
  int connection_north; //1 if wall, 0 if no wall
  int connection_east;
  int connection_west;
  int connection_south;
};

int is_in_range(int row, int col, int num_rows, int num_cols);

struct maze_room *get_neighbor(int num_rows,
        int num_cols, struct maze_room maze[][num_cols], struct maze_room *room, Direction dir);
*/
