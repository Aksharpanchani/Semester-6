#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine.h"

#define MAX_REFERENCES 1000


int argumentCheck(int totalFrames, char *filename, char *algo)
{
    // Checking if the total memory frames is within range
    if (totalFrames <= 0 || totalFrames > 100)
    {
        printf("Error: Total memory frames must be between 1 and 100.\n");
        return 1;
    }

    // Algorithm  validity check
    if (strcmp(algo, "opt") != 0 && strcmp(algo, "lru") != 0 && strcmp(algo, "fifo") != 0)
    {
        printf("Error: Invalid algorithm. Choose from opt, lru, or fifo.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // Checking for valid arguments
    if (argc != 4 && argc != 5)
    {
        printf("Invalid arguments\n");
        printf("Required arguments \n");
        printf("(a) The total number of physical memory frames (maximum 100)\n");
        printf("(b) Input filename \n");
        printf("(c) The chosen algorithm(opt, lru and fifo)\n");
        return 1;
    }

    int total_frames = atoi(argv[1]);
    char *filename = argv[2];
    char *algo = argv[3];
    char *doPrint = argv[4];

    if (argumentCheck(total_frames, filename, algo))
        return 1;

    if (doPrint == NULL)
    {
        printf("Total memory frames: %d\n", total_frames);
        printf("Input filename: %s\n", filename);
        printf("Chosen algorithm: %s\n", algo);
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: Could not open file %s.\n", filename);
        return 1;
    }

    int num[MAX_REFERENCES];
    int numLen = 0;
    while (fscanf(file, "%d", &num[numLen]) != EOF)
    {
        numLen++;
    }
    fclose(file);

    int frames[MAX_REFERENCES];

    int misses = 0;
    if (doPrint == NULL)
        printf("Memory Allocation State:\n");
    if (strcmp(algo, "fifo") == 0)
    {
        misses = fifo(total_frames, numLen, num, frames, doPrint);
        
    }
    else if (strcmp(algo, "lru") == 0)
    {
        misses = lru(total_frames, numLen, num, frames, doPrint);
        
    }
    else if (strcmp(algo, "opt") == 0)
    {
        misses = opt(total_frames, numLen, num, frames, doPrint);
        
    }
    float percent = (misses / (float)numLen) * 100;
    printf("Miss Rate = %d/%d = %.2f\n",misses, numLen, percent);
    return 0;
}
