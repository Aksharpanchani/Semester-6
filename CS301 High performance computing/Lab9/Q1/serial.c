#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define PI 3.14159265358979323846264338328

#define min(x, y) (((x) < (y)) ? (x) : (y))
//  Using the MONOTONIC clock
#define CLK CLOCK_MONOTONIC

/* Function to compute the difference between two points in time */
struct timespec diff(struct timespec start, struct timespec end);

struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0)
    {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    }
    else
    {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}


int main(int argc, char *argv[])
{
    struct timespec start_e2e, end_e2e, start_alg, end_alg, e2e, alg;
    /* Should start before anything else */
    clock_gettime(CLK, &start_e2e);

    /* Check if enough command-line arguments are taken in. */
    if (argc < 3)
    {
        printf("Usage: %s N p \n", argv[0]);
        return -1;
    }

    long long int N = atoi(argv[1]); /* size of input array */
    int P = atoi(argv[2]); /* number of processors*/
    char *problem_name = "PI_RANDOM";
    char *approach_name = "serial";
    //    char buffer[10];
    //    FILE* inputFile;
    FILE *outputFile;
    //    inputFile = fopen(argv[3],"r");

    char outputFileName[50];
    // sprintf(outputFileName,"output/%s_%s_%s_%s_output.txt",problem_name,approach_name,argv[1],argv[2]);
    snprintf(outputFileName, sizeof(outputFileName), "output/%s_%s_%s_%s_output.txt", problem_name, approach_name, argv[1], argv[2]);

    
    unsigned long long int circle_points = 0;
    unsigned int i;
    double rand_x, rand_y;
      srand((unsigned int)time(NULL));

    clock_gettime(CLK, &start_alg); /* Start the algo timer */
    /*----------------------Core algorithm starts here----------------------------------------------*/
 for (i = 0; i < N; i++) {
        rand_x = (double)rand() / RAND_MAX;
        rand_y = (double)rand() / RAND_MAX;

        double origin_dist = rand_x * rand_x + rand_y * rand_y;

        if (origin_dist <= 1)
            circle_points++;
    }

    double pi = (double)(4 * circle_points) / N;
    /*----------------------Core algorithm finished--------------------------------------------------*/

    clock_gettime(CLK, &end_alg); /* End the algo timer */
    /* Ensure that only the algorithm is present between these two
       timers. Further, the whole algorithm should be present. */
    /* Should end before anything else (printing comes later) */
    clock_gettime(CLK, &end_e2e);
    e2e = diff(start_e2e, end_e2e);
    alg = diff(start_alg, end_alg);

    /* problem_name,approach_name,N,p,e2e_sec,e2e_nsec,alg_sec,alg_nsec
       Change problem_name to whatever problem you've been assigned
       Change approach_name to whatever approach has been assigned
       p should be 0 for serial codes!!
     */
    printf("%s,%s,%d,%d,%ld,%ld,%ld,%ld\n", problem_name, approach_name, N, P, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);

	//printf("Estimated value of pi: %lf\n", pi);
    return 0;
}
