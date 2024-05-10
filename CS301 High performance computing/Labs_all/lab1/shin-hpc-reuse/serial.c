
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define dype double  

#define min(x, y) (((x) < (y)) ? (x) : (y))
//  Using the MONOTONIC clock
#define CLK CLOCK_MONOTONIC

/* Function to compute the difference between two points in time */
struct timespec diff(struct timespec start, struct timespec end);

void block_matmul(int n, dype **A, dype **B, dype **C, int block_size)
{
    int i, j, k, i1, j1, k1;
    for (i = 0; i < n; i += block_size)
    {
        for (j = 0; j < n; j += block_size)
        {
            for (k = 0; k < n; k += block_size)
            {
                for (i1 = i; i1 < i + block_size; i1++)
                {
                    for (j1 = j; j1 < j + block_size; j1++)
                    {
                        for (k1 = k; k1 < k + block_size; k1++)
                        {
                            C[i1][j1] += A[i1][k1] * B[k1][j1];
                        }
                    }
                }
            }
        }
    }
}

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
    dype **A = (dype **)malloc(N * sizeof(dype *));
    dype **B = (dype **)malloc(N * sizeof(dype *));
    dype **C = (dype **)malloc(N * sizeof(dype *));

    for (i = 0; i < N; i++)
    {
        A[i] = (dype *)malloc(N * sizeof(dype));
        B[i] = (dype *)malloc(N * sizeof(dype));
        C[i] = (dype *)malloc(N * sizeof(dype));
    }

    // Initialize the matrices
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            A[i][j] = 1;
            B[i][j] = 2;
            C[i][j] = 0;
        }
    }

    //***************

    clock_gettime(CLK, &start_alg); /* Start the algo timer */

    /*----------------------Core algorithm starts here----------------------------------------------*/

    // Matrix multiplication using Strassen's algorithm

    int block_size = 512;
    // if n is less than block size then use block size as n
    block_size = block_size< N ? block_size : N;

    block_matmul(N, A, B, C, block_size);


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
