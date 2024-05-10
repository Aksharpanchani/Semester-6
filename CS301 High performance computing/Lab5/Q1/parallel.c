
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define CLK CLOCK_MONOTONIC

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
	clock_gettime(CLK, &start_e2e);

	if (argc < 3)
	{
		printf("Usage: %s n p \n", argv[0]);
		return -1;
	}

	int N = atoi(argv[1]); /* size of input array */
	int P = atoi(argv[2]); /* number of processors*/
	char *problem_name = "matrix_multiplication";
	char *approach_name = "block";

	FILE *outputFile;

	char outputFileName[50];
	sprintf(outputFileName, "output/%s_%s_%s_%s_output.txt", problem_name, approach_name, argv[1], argv[2]);

	//***************
	// Problem - 1 Matrix Multiplication

	// Matrix A of size N*N and Matrix B of size N*N are multiplied to get Matrix C of size N*N

	int i, j, k;

	// using dynamic memory allocation as memory required is very large for large values of N, and stack memory is limited in size so it will cause stack overflow and we will get segmentation fault
	// heap memory is very large in size so we can use it to allocate memory for large values of N
	double **A = (double **)malloc(N * sizeof(double *));
	double **B = (double **)malloc(N * sizeof(double *));
	double **C = (double **)malloc(N * sizeof(double *));
	for (i = 0; i < N; i++)
	{
		A[i] = (double *)malloc(N * sizeof(double));
		B[i] = (double *)malloc(N * sizeof(double));
		C[i] = (double *)malloc(N * sizeof(double));
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

	//***************

	clock_gettime(CLK, &start_alg); /* Start the algo timer */

	/*----------------------Core algorithm starts here----------------------------------------------*/

	// Matrix Multiplication Algorithm
	omp_set_num_threads(P);
	#pragma omp parallel for private(i, k, j) shared(A, B, C) collapse(2)
	for (i = 0; i < N; i++)
	{
		for (k = 0; k < N; k++)
		{
			float sum = 0.0f;
			#pragma omp reduction(+:sum)
			for (j = 0; j < N; j++)
			{
				sum += A[i][k] * B[k][j];
			}
			C[i][j] = sum;
		}
	}

	/*----------------------Core algorithm finished--------------------------------------------------*/

	clock_gettime(CLK, &end_alg); /* End the algo timer */

	clock_gettime(CLK, &end_e2e);
	e2e = diff(start_e2e, end_e2e);
	alg = diff(start_alg, end_alg);

	printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, N, P, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);

	return 0;
}