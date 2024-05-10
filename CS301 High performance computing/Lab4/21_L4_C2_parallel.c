#include<stdio.h>
#include<math.h>
#include<omp.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))
// Using the MONOTONIC clock
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
struct timespec diff(struct timespec start, struct timespec end){
    struct timespec temp;
    if((end.tv_nsec-start.tv_nsec)<0){
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    }
    else{
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

void Merge(double ar[], long int l,long int m, long int r) 
{ 
    long int i, j, k; 
    long int n_1 = m - l + 1; 
    long int n_2 = r - m; 
  
    // Create temp arrays 
    double *L, *R;
    L = (double*)malloc(n_1*sizeof(double));
    R = (double*)malloc(n_2*sizeof(double));
  
    // Copy data to temp arrays L[] and R[] 
    for (i = 0; i < n_1; i++) 
        L[i] = ar[l + i]; 
    for (j = 0; j < n_2; j++) 
        R[j] = ar[m + 1 + j]; 
  
    // Merge the temp arrays back into ar[l..r] 
    // Initial index of first subarray 
    i = 0; 
    // Initial index of second subarray 
    j = 0; 
    // Initial index of merged subarray 
    k = l; 
    while (i < n_1 && j < n_2) { 
        if (L[i] <= R[j]) { 
            ar[k] = L[i]; 
            i++; 
        } 
        else { 
            ar[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
    while (i < n_1) { 
        ar[k] = L[i]; 
        i++; 
        k++; 
    } 
    while (j < n_2) { 
        ar[k] = R[j]; 
        j++; 
        k++; 
    } 
    free(L);
    free(R);
} 
  
// l is for left index and r is right index of the sub-array of ar to be sorted 
void merge_Sort(double ar[], long int l, long int r) 
{ 
    if (l < r) { 
        // Same as (l+r)/2, but avoids overflow for large l and r 
        long int m = l + (r - l) / 2; 
        // Sort first and second halves in parallel
        #pragma omp parallel sections
        {
            #pragma omp section
            merge_Sort(ar, l, m); 
            #pragma omp section
            merge_Sort(ar, m + 1, r); 
        }
        Merge(ar, l, m, r); 
    } 
} 

int main(int argc, char* argv[])
{
    struct timespec start_e2e, end_e2e, start_alg, end_alg, e2e, alg;
    /* Should start before anything else */
    clock_gettime(CLK, &start_e2e);

    /* Check if enough command-line arguments are taken in. */
    if(argc < 3){
        printf( "Usage: %s n p \n", argv[0] );
        return -1;
    }

    int N=atoi(argv[1]);    /* size of input array */
    int P=atoi(argv[2]);    /* number of processors*/
    char *prob_name = "matrix_multiplication";
    char *app_name = "block";
//  char buffer[10];
//  FILE* inputFile;
    FILE* outputFile;
    //  inputFile = fopen(argv[3],"r");

    char *outputFileName;
    asprintf(&outputFileName, "output/%s_%s_%s_%s_output.txt", prob_name, app_name, argv[1], argv[2]);

    /*----------------INPUT/OUPUT operations start here-----------------------------*/

    long int i;
    double sum=0.0, terms, pi;
    long int num;
    double* ar;
    ar = (double*)malloc(N*sizeof(double));
    for(i=0;i<N;++i){
        ar[i] = (double)(rand());
    }

    /*----------------INPUT/OUPUT operations end here-----------------------------*/
    
    clock_gettime(CLK, &start_alg);    /* Start the algo timer */

    /*----------------------Core algorithm starts here----------------------------------------------*/

    // Parallel Merge sort
    merge_Sort(ar,0,N-1);

    /*----------------------Core algorithm finished--------------------------------------------------*/

    clock_gettime(CLK, &end_alg);    /* End the algo timer */
    /* Ensure that only the algorithm is present between these two
       timers. Further, the whole algorithm should be present. */


    /* Should end before anything else (printing comes later) */
    clock_gettime(CLK, &end_e2e);
    e2e = diff(start_e2e, end_e2e);
    alg = diff(start_alg, end_alg);

    /* prob_name,app_name,n,p,e2e_sec,e2e_nsec,alg_sec,alg_nsec
       Change prob_name to whatever problem you've been assigned
       Change app_name to whatever approach has been assigned
       p should be 0 for serial codes!! 
     */
    printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", prob_name, app_name, N, P, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);

    free(ar); // Free dynamically allocated memory

    return 0;

}
