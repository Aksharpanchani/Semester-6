#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REFERENCES 1000
// Akshar Panchani //

int argchk(int pagerefs, int sq_len)
{
    // Checking if the total memory frames is within range
    if (pagerefs <= 0 || pagerefs > 500)
    {
        printf("Error: Page references should be in [1,500].\n");
        return 1;
    }
    return 0;
}

void prtrndms(int sq_len, int nms[])
{
    for (int i = 0; i < sq_len;)
    {
        int temp = (rand() % (10));
        if (i == 0 || temp != nms[i - 1])
        {
            nms[i] = temp;
            i++;
        }

        // printf("%d ", nms[i]);
    }
}

int main(int argc, char *argv[])
{
    // Checking for valid arguments
    if (argc != 4)
    {
        printf("Invalid arguments\n");
        printf("Required arguments \n");
        printf("Range of page references max 500)\n");
        printf("Length of the sequence \n");
        printf("Name of the file that will be generated\n");
        return 1;
    }

    
    // Storing arguments
    int pagerefs = atoi(argv[1]);
    int sq_len = atoi(argv[2]);
    char *flname = argv[3];
    

    if (argchk(pagerefs, sq_len))
        return 1;

    // Here you can implement the logic for your vmsim program based on the inputs
    printf("Page Reference: %d\n", pagerefs);
    printf("Sequence Length: %d\n", sq_len);
    printf("File name: %s\n", flname);

    int nms[sq_len];
    prtrndms(sq_len, nms);
    for (int i = 0; i < sq_len; i++)
        printf("%d ", nms[i]);

    // Writing data into file
    
    FILE *file = fopen(flname, "w");


    for (int i = 0; i < sq_len; i++)
        fprintf(file, "%d ", nms[i]);

    fclose(file);

    printf("File generated successfully.\n");

    return 0;
}
