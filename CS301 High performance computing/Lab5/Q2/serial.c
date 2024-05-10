
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define dtype double


#define min(x, y) (((x) < (y)) ? (x) : (y))
//  Using the MONOTONIC clock
#define CLK CLOCK_MONOTONIC

/* Function to compute the difference between two points in time */
struct timespec diff(struct timespec start, struct timespec end);

/*
   Function to computes the difference between two time instances

   Taken from - http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/

   Further reading:
http://stackoverflow.com/questions/6749621/how-to-create-a-high-resolution-timer-in-linux-to-measure-program-performance
http://stackoverflow.com/questions/3523442/difference-between-clock-realtime-and-clock-monotonic
 */
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
		printf("Usage: %s n p \n", argv[0]);
		return -1;
	}

	int N = atoi(argv[1]); /* size of input array */
	int P = atoi(argv[2]); /* number of processors*/
	char *problem_name = "matrix_multiplication";
	char *approach_name = "block";
	//	char buffer[10];
	//	FILE* inputFile;
	FILE *outputFile;
	//	inputFile = fopen(argv[3],"r");

	char outputFileName[50];
	sprintf(outputFileName, "output/%s_%s_%s_%s_output.txt", problem_name, approach_name, argv[1], argv[2]);

	//***************
	// Problem - 1 Matrix Multiplication

	// Matrix A of size N*N and Matrix B of size N*N are multiplied to get Matrix C of size N*N

	int i, j, k;
	dtype **A = (dtype **)malloc(N * sizeof(dtype *));
	dtype **B = (dtype **)malloc(N * sizeof(dtype *));
	dtype **C = (dtype **)malloc(N * sizeof(dtype *));

	for (i = 0; i < N; i++)
	{
		A[i] = (dtype *)malloc(N * sizeof(dtype));
		B[i] = (dtype *)malloc(N * sizeof(dtype));
		C[i] = (dtype *)malloc(N * sizeof(dtype));
	}

	// Initialize the matrices
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			A[i][j] = 2;
			B[i][j] = 1;
			C[i][j] = 0;
		}
	}
	dtype **B_transpose = (dtype **)malloc(N * sizeof(dtype *));
	for (i = 0; i < N; i++)
	{
		B_transpose[i] = (dtype *)malloc(N * sizeof(dtype));
	}

	// Calculate B_transpose, first convert B to 1-D array
	dtype *B_1D = (dtype *)malloc(N * N * sizeof(dtype));
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			B_1D[i * N + j] = B[i][j];
		}
	}

	// Transpose B_1D to get B_transpose_1D and then convert it back to 2-D array
	dtype *B_transpose_1D = (dtype *)malloc(N * N * sizeof(dtype));
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			B_transpose_1D[j * N + i] = B_1D[i * N + j];
		}
	}

	// Convert B_transpose_1D to 2-D array
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			B_transpose[i][j] = B_transpose_1D[i * N + j];
		}
	}
	//***************
	clock_gettime(CLK, &start_alg); /* Start the algo timer */

	/*----------------------Core algorithm starts here----------------------------------------------*/

	// Matrix Multiplication by calculating B_transpose and then multiplying it with A

	// Multiply A and B_transpose
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			for (k = 0; k < N; k++)
			{
				C[i][j] += A[i][k] * B_transpose[j][k];
			}
		}
	}

	/*----------------------Core algorithm finished--------------------------------------------------*/

	clock_gettime(CLK, &end_alg); /* End the algo timer */
	/* Ensure that only the algorithm is present between these two
	   timers. Further, the whole algorithm should be present. */

	/* Should end before anything else (printing comes later) */
	clock_gettime(CLK, &end_e2e);
	e2e = diff(start_e2e, end_e2e);
	alg = diff(start_alg, end_alg);

	/* problem_name,approach_name,n,p,e2e_sec,e2e_nsec,alg_sec,alg_nsec
	   Change problem_name to whatever problem you've been assigned
	   Change approach_name to whatever approach has been assigned
	   p should be 0 for serial codes!!
	 */
	printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, N, P, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);

	return 0;
}