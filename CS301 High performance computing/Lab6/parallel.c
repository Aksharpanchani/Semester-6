#include <stdio.h>
#include <math.h>
#include <omp.h>
#include<time.h>
#include <string.h>
#include <stdlib.h>

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

void transpose_cache(double **src, double **dst, int n)
{
    // Define a block size for cache optimization
    int blocksize = 32;
    int i, j, k, l;

    // Iterate over the matrix in blocks
    for (i = 0; i < n; i += blocksize)
    {
        for (j = 0; j < n; j += blocksize)
        {
            // Transpose the current block
            for (k = i; k < i + blocksize && k < n; ++k)
            {
                for (l = j; l < j + blocksize && l < n; ++l)
                {
                    dst[k][l] = src[l][k];
                }
            }
        }
    }
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

    int N = atoi(argv[1]); /* size of input array */
    int P = atoi(argv[2]); /* number of processors*/
    char *problem_name = "matrix_multiplication";
    char *approach_name = "block";
    //	char buffer[10];
    //	FILE* inputFile;
    FILE *outputFile;
    //	inputFile = fopen(argv[3],"r");

    char outputFileName[50];
    // sprintf(outputFileName,"output/%s_%s_%s_%s_output.txt",problem_name,approach_name,argv[1],argv[2]);
    snprintf(outputFileName, sizeof(outputFileName), "output/%s_%s_%s_%s_output.txt", problem_name, approach_name, argv[1], argv[2]);

    //***************
    int i, j, k, row, col; // loop variables

    double **a = (double **)malloc(N * sizeof(double *));
    for (i = 0; i < N; i++)
    {
        a[i] = (double *)malloc(N * sizeof(double));
    }
    double **q = (double **)malloc(N * sizeof(double *));
    for (i = 0; i < N; i++)
    {
        q[i] = (double *)malloc(N * sizeof(double));
    }
    double **r = (double **)malloc(N * sizeof(double *));
    for (i = 0; i < N; i++)
    {
        r[i] = (double *)malloc(N * sizeof(double));
    }
    double **a_transpose = (double **)malloc(N * sizeof(double *)), **q_transpose = (double **)malloc(N * sizeof(double *));
    for (row = 0; row < N; row++)
    {
        a_transpose[row] = (double *)malloc(N * sizeof(double));
        q_transpose[row] = (double *)malloc(N * sizeof(double));
    }

    srand(time(0));
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            a[i][j] = rand() % 10;
        }
    }

    double s;
    clock_gettime(CLK, &start_alg); /* Start the algo timer */
    /*----------------------Core algorithm starts here----------------------------------------------*/

    // Allocate memory for the transpose of matrix a
    // Compute the transpose of matrix a
    transpose_cache(a, a_transpose, N);
    // Compute the R matrix
    for (i = 0; i < N; i++)
    {
        // Compute the diagonal element of R
        double sum = 0;
        for (j = 0; j < N; j++)
        {
            sum += a_transpose[i][j] * a_transpose[i][j];
        }
        r[i][i] = sqrt(sum);

        // Compute the i-th column of Q
        for (j = 0; j < N; j++)
        {
            q_transpose[i][j] = a_transpose[i][j] / r[i][i];
        }

        // Update the remaining columns of R and A_T
        for (j = i + 1; j < N; j++)
        {
            // Compute the off-diagonal elements of R
            double sum = 0;
            for (k = 0; k < N; k++)
            {
                sum += q_transpose[i][k] * a_transpose[j][k];
            }
            r[i][j] = sum;

            // Update A_T by subtracting the projection of column j onto column i
            for (k = 0; k < N; k++)
            {
                a_transpose[j][k] -= sum * q_transpose[i][k];
            }
        }
    }
    // Transpose the Q matrix to obtain an orthogonal matrix
    transpose_cache(q_transpose, q, N);
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
    printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, N, P, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);

    return 0;
}
