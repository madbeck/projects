// a sequential program that simulates random dart throws (using normal distribution) and accepts user input for varying skill and aim
// goal is to simulate enough throws to guage which aims work best for different skill levels

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

double generateGaussianNoise(double mu, double sigma) //below from https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform#Implementation
{
	const double epsilon = DBL_MIN;
	const double two_pi = 2.0*3.14159265358979323846;
	static double z0, z1;	
	static int generate=0; //starts out as false, but everytime I recall the function, it doesn't get redefined, retains previous definitio
	generate = !generate; //flips btwn true and false

	if (!generate)
	   return z1 * sigma + mu;
	double u1, u2;
	do
	 {
	   u1 = rand() * (1.0 / RAND_MAX); //sets u1 to a random number between 0 and 1
	   //remember RAND_MAX is just the largest random number we can generate between 0 and 1
	   u2 = rand() * (1.0 / RAND_MAX);
	 }

	while ( u1 <= epsilon );
	// uses random u1 and u2 to do gaussian distribution below
	z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2); //sets z0 and z1 to uniformly distributed variables in (0,1)
	z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
	return z0 * sigma + mu;
}

int main()
{
	time_t t;
	srand(time(NULL));
	const double two_pi = 2.0*3.14159265358979323846;
	const double pi = 3.14159265358979323846;
	double x, y;
	int j, i, n, num = 10000000; //num is number of throws
	n = num;
	int skill;
	int aim;
	double level, x_aim, y_aim, extra;
	printf("What is your skill level? Please choose a level between 1 and 10, 1 being the lowest skill level, and 10 being the highest.\n");
	scanf("%i",&skill);
	
  if(skill==1){
		level = 0.4;
	}
	if(skill==2){
		level = 0.3;
	}
	if(skill==3){
		level = 0.2;
	}
	if(skill==4){
		level = 0.1;
	}
	if(skill==5){
		level = 0.075;
	}
	if(skill==6){
		level = 0.05;
	}
	if(skill==7){
		level = 0.033;
	}
	if(skill==8){
		level = 0.025;
	}
	if(skill==9){
		level = 0.01;
	}
	if(skill==10){
		level = 0.001;
	}
  
	printf("Where would you like to aim? You can aim at the 20, the 5, or the 1 slice.\n");
	scanf("%i",&aim); //aiming will always be in the 3-point score part of the slice
	if(aim==20){
		x_aim = 0.5;
		y_aim = 0.5+(131.0/432);
	}
	if(aim==5){
		x_aim = 0.5+((131.0/432)*cos(3.0*pi/5));
		y_aim = 0.5+((131.0/432)*sin(3.0*pi/5));
	}
	if(aim==1){
		x_aim = 0.5+((131.0/432)*cos(2.0*pi/5));
		y_aim = 0.5+((131.0/432)*sin(2.0*pi/5));
	}
	printf("x_aim: %lf\n", x_aim);
	printf("y_aim: %lf\n", y_aim);
	printf("level: %lf\n", level);
  
	long int score=0, total_score=0;
	clock_t begin = clock();
	for( i = 0 ; i < n ; i++)
	{
		x = generateGaussianNoise(x_aim, level); //changes based on user input for aim and skill level
		y = generateGaussianNoise(y_aim, level);
		double theta;
		double r, new_x, new_y, center_x = 0.5, center_y = 0.5; //center_x and center_y are constants for center of circle
		new_x = x - center_x; //these are redefined later too
		new_y = y - center_y;
		r = sqrt(pow(new_x, 2) + pow(new_y, 2)); //radius will always be positive
    
		if((r > 0.5) || (x<0) || (y<0) || (x>1) || (y>1)) {
			score = 0;
		}
		else if((0.0<=r)&&r<=(1.0/54)){
			score = 50; //bullseye
		}
		else if((1.0/54)<=r&&r<=(5.0/54)){
			score = 25; //outer bullseye-ring
		}
		else {
			assert(r <= 0.5);
			//4 quadrants, so 4 cases:
			if (((0.5<= x) && (x <= 1)) && ((0.5<=y) && (y<=1))){
				new_x = x - center_x;
				new_y = y - center_y;
				theta = atan(new_y/new_x);
			}
			else if(0<=x && x<0.5 && 0.5<=y && y<=1){
				new_x = center_x - x;
				new_y = y - center_y;
				theta = pi - atan(new_y/new_x);
			}
			else if(0<=x && x<0.5 && 0<=y && y<0.5){
				new_x = center_x - x;
				new_y = center_y - y;
				theta = atan(new_y/new_x) + pi;
			}
			else if(0.5<=x && x<=1 && 0<=y && y<0.5){
				new_x = x - center_x;
				new_y = center_y - y;
				theta = (2.0*pi) - atan(new_y/new_x);
			}
			else {
				printf("uh oh, here's r: %f\n", r);
			}
			if ((0.0<=theta)&&(theta<=(pi)/20)){
				score = 6;
			}
			else if((((pi)/20)<theta)&&(theta<=(3*(pi)/20))){
				score = 13;
			}			
			else if(((3*(pi)/20)<theta)&&(theta<=(5*(pi)/20))){
				score = 4;
			}
			else if((5*(pi)/20<theta)&&(theta<=7*(pi)/20)){
				score = 18;
			}
			else if((7*(pi)/20<theta)&&(theta<=9*(pi)/20)){
				score = 1;
			}
			else if((9*(pi)/20<theta)&&(theta<=11*(pi)/20)){
				score = 20;
			}
			else if((11*(pi)/20<theta)&&(theta<=13*(pi)/20)){
				score = 5;
			}
			else if((13*(pi)/20<theta)&&(theta<=15*(pi)/20)){
				score = 12;
			}
			else if((15*(pi)/20<theta)&&(theta<=17*(pi)/20)){
				score = 9;
			}
			else if((17*(pi)/20<theta)&&(theta<=19*(pi)/20)){
				score = 14;
			}
			else if((19*(pi)/20<theta)&&(theta<=21*(pi)/20)){
				score = 11;
			}
			else if((21*(pi)/20<=theta)&&(theta<23*(pi)/20)){
				score = 8;
			}
			else if((23*(pi)/20<=theta)&&(theta<25*(pi)/20)){
				score = 16;
			}
			else if((25*(pi)/20<=theta)&&(theta<27*(pi)/20)){
				score = 7;
			}
			else if((27*(pi)/20<=theta)&&(theta<29*(pi)/20)){
				score = 19;
			}
			else if((29*(pi)/20<=theta)&&(theta<31*(pi)/20)){
				score = 3;
			}
			else if((31*(pi)/20<=theta)&&(theta<33*(pi)/20)){
				score = 17;
			}
			else if((33*(pi)/20<=theta)&&(theta<35*(pi)/20)){
				score = 2;
			}
			else if((35*(pi)/20<=theta)&&(theta<37*(pi)/20)){
				score = 15;
			}
			else if((37*(pi)/20<=theta)&&(theta<39*(pi)/20)){
				score = 10;
			}
			else if((39*(pi)/20<=theta)&&(theta<40*(pi)/20)){
				score = 6;
			}
			else {
				printf("uh oh, r: %f\n", r); //a random variable
				printf("uh oh, og_x: %lf\n", x);
				printf("uh oh, og_y: %lf\n", y);
				printf("uh oh, x: %lf\n", new_x);
				printf("uh oh, y: %lf\n", new_y);
				printf("uh oh, theta: %lf\n", theta);
			}	
		}
		if(((126.0/432)<=r)&&(r<=(136.0/432))){
			score = score*3; //triple-score ring
		}
		else if(((206.0/432)<=r)&&(r<=1.0)){
			score = score*2; //double-score ring
		}
		total_score = total_score + score;
	}
	printf("The total score is: %ld\n", total_score);
	double avg_score=0.0;
	avg_score = (1.0*(total_score)/(1.0 * num));
	printf("The average score is: %lf\n", avg_score);
	clock_t end = clock();
	double time_spent = (double) (end - begin)/CLOCKS_PER_SEC;
	printf("time: %lf\n", time_spent);
}
