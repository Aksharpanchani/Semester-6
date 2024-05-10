
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define dtype double

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

void block_matmul(int n, dtype **A, dtype **B, dtype **C, int block_size)
{
    int i, j, k, ii, jj, kk;
    #pragma omp parallel for private(i,j,k) shared(A,B,C,n) collapse(2) 
    for (ii = 0; ii < n; ii += block_size)
    {
        for (jj = 0; jj < n; jj += block_size)
        {
            for (kk = 0; kk < n; kk += block_size)
            {
                for (i = ii; i < min(ii + block_size,n); i++)
                {
                    for (j = jj; j < min(jj + block_size,n); j++)
                    {
                        double sum = 0;
                        #pragma omp simd reduction(+:sum)
                        for (k = kk; k < min(kk + block_size,n); k++)
                        {
                            sum += A[i][k] * B[k][j];
                        }
                        C[i][j] += sum;
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
	struct timespec start_e2e, end_e2e, start_alg, end_alg, e2e, alg;
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
	FILE *outputFile;

	char outputFileName[50];
	sprintf(outputFileName, "output/%s_%s_%s_%s_output.txt", problem_name, approach_name, argv[1], argv[2]);

	//***************
	// Matrix A of size N*N and Matrix B of size N*N are multiplied to get Matrix C of size N*N

	int i, j, k;

	// using dynamic memory allocation as memory required is very large for large values of N, and stack memory is limited in size so it will cause stack overflow and we will get segmentation fault
	// heap memory is very large in size so we can use it to allocate memory for large values of N

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
#pragma omp parallel for private(i, j) collapse(2)
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

	int block_size = 32;
	omp_set_num_threads(P);
	// if n is less than block size then use block size as n
	block_size = block_size < N ? block_size : N;

	omp_set_num_threads(P);
	block_matmul(N, A, B, C, block_size);

	/*----------------------Core algorithm finished--------------------------------------------------*/

	clock_gettime(CLK, &end_alg); /* End the algo timer */

	clock_gettime(CLK, &end_e2e);
	e2e = diff(start_e2e, end_e2e);
	alg = diff(start_alg, end_alg);

	printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, N, P, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);

	return 0;
}