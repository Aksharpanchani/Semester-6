#include <stdio.h>
#include <math.h>
#include <time.h>
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

// Utility function to swp two integers
void swp(double *p_1, double *p_2)
{
	double temp;
	temp = *p_1;
	*p_1 = *p_2;
	*p_2 = temp;
}

int parting(double arr[], int low, int high)
{
	// choose the piv
	double piv = arr[high];
	int i = low - 1; // Index of smaller element
	int j;
	for (j = low; j <= high - 1; j++)
	{
		// If current element is smaller than the piv
		if (arr[j] < piv)
		{
			i++;
			swp(&arr[i], &arr[j]);
		}
	}
	swp(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void quick_sort(double arr[], int low, int high, double targt, int *res)
{
	if (low < high)
	{
		int pi = parting(arr, low, high);

		if (arr[pi] == targt) // If piv element is the targt
		{
			*res = pi;
			return;
		}

		if (arr[pi] < targt) // If piv is less than targt, search on right side
			quick_sort(arr, pi + 1, high, targt, res);
		else // If piv is greater than targt, search on left side
			quick_sort(arr, low, pi - 1, targt, res);
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
	char *problem_name = "BinarySearch";
	char *approach_name = "Serial";
	//    char buffer[10];
	//    FILE* inputFile;
	FILE *outputFile;
	//    inputFile = fopen(argv[3],"r");

	char outputFileName[50];
	// sprintf(outputFileName,"output/%s_%s_%s_%s_output.txt",problem_name,approach_name,argv[1],argv[2]);
	snprintf(outputFileName, sizeof(outputFileName), "output/%s_%s_%s_%s_output.txt", problem_name, approach_name, argv[1], argv[2]);

	/*----------------INPUT/OUPUT operations start here-----------------------------*/

	srand(time(NULL));
	int i;
	double *a1 = (double *)malloc(N * sizeof(double));
	double targt = rand() % 100; // Change the targt value as needed
	int res = -1;
	if (a1 == NULL)
	{
		printf("Memory allocation failed\n");
		return -1;
	}

	for (i = 0; i < N; i++)
	{
		a1[i] = N - i;
	}

	// printf("Unsorted Array\n");
	// for (i = 0; i < N; i++)
	// {
	// 	printf("%f ", a1[i]);
	// }

	/*----------------INPUT/OUPUT operations end here-----------------------------*/

	clock_gettime(CLK, &start_alg); /* Start the algo timer */
	/*----------------------Core algorithm starts here----------------------------------------------*/

	quick_sort(a1, 0, N - 1, targt, &res);

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

	// if (res != -1)
	//     printf("Element %f found at index %d\n", targt, res);
	// else
	//     printf("Element %f not found\n", targt);

	// printf("Sorted Array\n");
	// for (i = 0; i < N; i++)
	// {
	//     printf("%f ", a1[i]);
	// }

	free(a1);

	return 0;
}
