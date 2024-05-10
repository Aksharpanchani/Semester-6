#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define PI 3.14159265358979323846264338328
#define min(x, y) (((x) < (y)) ? (x) : (y))
//  Using the MONOTONIC clock
#define CLK CLOCK_MONOTONIC

/* Function to compute the difference between two points in time */
struct timespec diff(struct timespec start, struct timespec end);

/*
   Function to computes the difference between two time instances

   Taken from - http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/

   Further reading:
http://stackoverflow.com/questions/6749621/how-to-create-aa-high-resolution-timer-in-linux-to-measure-program-performance
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
int number_of_threads;
// Define your own structure for complex numbers
typedef struct Complex
{
    double rel1;
    double img1;
} Complex;

void fft(Complex *x, int N)
{
    // DFT
    unsigned int k = N, n;
    double theTa = PI / N; // Adjust theTa to use positive values
    Complex Phit = (Complex){cos(theTa), -sin(theTa)}, T;
    while (k > 1)
    {
        n = k;
        k >>= 1;
        Phit = Phit = (Complex){Phit.rel1 * Phit.rel1 - Phit.img1 * Phit.img1, 2 * Phit.rel1 * Phit.img1};
        // Adjust Phit calculation
        T = (Complex){1.0, 0.0};
        unsigned int l, aa;

        for (l = 0; l < k; l++)
        {
#pragma omp parallel for num_threads(number_of_threads) schedule(static, 4)
            for (aa = l; aa < N; aa += n)
            {
                unsigned int b1 = aa + k;
                Complex t = {x[aa].rel1 - x[b1].rel1, x[aa].img1 - x[b1].img1};
                x[aa].rel1 += x[b1].rel1;
                x[aa].img1 += x[b1].img1;
                x[b1] = (Complex){t.rel1 * T.rel1 - t.img1 * T.img1, t.rel1 * T.img1 + t.img1 * T.rel1};
            }
            T = (Complex){T.rel1 * Phit.rel1 - T.img1 * Phit.img1, T.rel1 * Phit.img1 + T.img1 * Phit.rel1};
        }
    }
    // Decimate
    unsigned int m1 = log2(N);
    unsigned int aa;
#pragma omp parallel for num_threads(number_of_threads) schedule(static, 4)
    for (aa = 0; aa < N; aa++)
    {
        unsigned int b1 = aa;
        // Reverse bits
        b1 = (((b1 & 0xaaaaaaaa) >> 1) | ((b1 & 0x55555555) << 1));
        b1 = (((b1 & 0xcccccccc) >> 2) | ((b1 & 0x33333333) << 2));
        b1 = (((b1 & 0xf0f0f0f0) >> 4) | ((b1 & 0x0f0f0f0f) << 4));
        b1 = (((b1 & 0xff00ff00) >> 8) | ((b1 & 0x00ff00ff) << 8));
        b1 = ((b1 >> 16) | (b1 << 16)) >> (32 - m1);
        if (b1 > aa)
        {
            Complex t = x[aa];
            x[aa] = x[b1];
            x[b1] = t;
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
        printf("Usage: %s n p \n", argv[0]);
        return -1;
    }

    int N = atoi(argv[1]); /* size of input array */
    int P = atoi(argv[2]); /* number of processors*/
    char *problem_name = "FFT";
    char *approach_name = "block";
    //	char buffer[10];
    //	FILE* inputFile;
    FILE *outputFile;
    //	inputFile = fopen(argv[3],"r");

    char *outputFileName;
    asprintf(&outputFileName, "output/%s_%s_%s_%s_output.txt", problem_name, approach_name, argv[1], argv[2]);

    /*----------------INPUT/OUPUT operations start here-----------------------------*/

    Complex *input = (Complex *)malloc(N * sizeof(Complex));

    // Initialize input sequence
    int i;
    // printf("Input sequence:\n");
    for (i = 0; i < N; i++)
    {
        input[i] = (Complex){i, 0}; // Real part is 'i' and imaginary part is 0
        // printf("%.2f + %.2fi\n", input[i].rel1, input[i].img1);
    }
    /*----------------INPUT/OUPUT operations end here-----------------------------*/

    clock_gettime(CLK, &start_alg); /* Start the algo timer */

    /*----------------------Core algorithm starts here----------------------------------------------*/
    number_of_threads = P;

    fft(input, N);

    /*----------------------Core algorithm finished--------------------------------------------------*/

    clock_gettime(CLK, &end_alg); /* End the algo timer */
    /* Ensure that only the algorithm is present between these two
       timers. Further, the whole algorithm should be present. */

    /* Should end before anything else (printing comes later) */
    clock_gettime(CLK, &end_e2e);
    e2e = diff(start_e2e, end_e2e);
    alg = diff(start_alg, end_alg);


    printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, N, P, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);

   
    free(input);
    return 0;
}
