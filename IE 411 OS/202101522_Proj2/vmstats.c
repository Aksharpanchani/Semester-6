#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int main(int argc, char *argv[])
{
    if (argc != 5)
    {
  printf("Invalid arguments\n");
        printf("Required arguments \n");
             printf("Name of the file that will be generated\n");
        printf("no less than 1\n");
        printf("no more than 500\n");
        
        return 1;
    }

    int minfrms = atoi(argv[1]);
    int maxfrms = atoi(argv[2]);
    int incrmts = atoi(argv[3]);
    char *flname = argv[4];

    FILE *file = fopen(flname, "r");
    if (file == NULL)
    {
        printf("Error: Could not open file %s.\n", flname);
        return 1;
    }

    int num[1000];
    int numblen = 0;
    while (fscanf(file, "%d", &num[numblen]) != EOF)
    {
        numblen++;
    }
    fclose(file);

    char algorthm[3][5] = {"opt", "lru", "fifo"};
    int frms[100];
    int mss = 0;

    FILE *out = fopen("vmrates.dat", "w");
    if (out == NULL)
    {
        printf("Error: Could not open file %s.\n", "vmrates.dat");
        return 1;
    }
    for (int j = minfrms; j <= maxfrms; j += incrmts)
    {
        fprintf(out, "%d ", j);
    }
    fprintf(out, "\n");

    for (int i = 0; i < 3; i++)
    {
        for (int j = minfrms; j <= maxfrms; j += incrmts)
        {
            if (i == 0)
                mss = opt(j, numblen, num, frms, "s");
            else if (i == 1)
                mss = lru(j, numblen, num, frms, "s");
            else
                mss = fifo(j, numblen, num, frms, "s");

            float percnt = (mss / (float)numblen) * 100;
            fprintf(out, "%.2f ", percnt);
            printf("%s, %d frms, Miss Rate = %d/%d = %.2f\n", algorthm[i], j, mss,
                   numblen, percnt);
        }
        fprintf(out, "\n");
    }
    fclose(out);

    return 0;
}