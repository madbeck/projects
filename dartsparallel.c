// a parallel version of the sequential darts code
// uses OpenMP with up to 8 threads to do this
// focus is on timing for a large number of throws so aim and skill level are constant

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>
#include <limits.h>
#include "cur_time.h" //cure_time.h is a separate file that computes time more accurately for parallel programs

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
       u1 = drand48() * (1.0 / RAND_MAX); //sets u1 to a random number between 0 and 1, using drand48() because it's supposed to be thread-safe
       //drand48() is a thread-safe number generating function
       u2 = drand48() * (1.0 / RAND_MAX);
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
    int j, i, n, num = 1000000; //number of dart throws
    n = num;
    int skill;
    int aim;
    double level, x_aim, y_aim, extra;
    int num_procs = 8; //changes number of processors
  
    printf("A parallel program to throw %d darts at a board", num);
    printf(" using %i processor(s).\n", num_procs);
    printf("Number of processors available = %d\n", omp_get_num_procs());
    printf("Number of threads = %d\n", omp_get_max_threads());
  
    double score=0, total_score=0;
    int tid;
    double start_time = cur_time(); //starts clock
    #pragma omp parallel num_threads(num_procs)
    {
    #pragma omp for reduction(+:total_score)
    for (i=0; i < n ; i++) {
        tid = omp_get_thread_num();
        x = generateGaussianNoise(0.5, 0.001);
        y = generateGaussianNoise((0.5+(131.0/432)), 0.001);
        double theta;
        double r, new_x, new_y, center_x = 0.5, center_y = 0.5; //center_x and center_y are constants for center of circle
        new_x = x - center_x; //these are redefined later
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
            if(((126.0/432)<=r)&&(r<=(136.0/432))){
                score = score*3; //triple-score ring
            }
            else if(((206.0/432)<=r)&&(r<=1.0)){
                score = score*2; //double-score ring
            }
            total_score = total_score + score;  
        }
    }
    int num_threads;
    num_threads = omp_get_num_threads();
    printf("how many threads: %i\n", num_threads);
    }
    double run_time = cur_time() - start_time;
    double elapsed_time = 1.0 * (run_time);
    double avg_score=0.0;
    avg_score = (1.0*(total_score)/(1.0 * num));
    printf("The average score is: %lf\n", avg_score);
    //printf("number of threads: %i\n", num_threads);
    printf("time: %lf\n", elapsed_time);
}
