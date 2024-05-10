#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to print a particular answer
//This is used to print answer as given in then the file 
void print_answer(int frm[], int t_frame, int p_f, int start)  // p_f is the page fault
{
    printf("[");
    for (int i = 0; i < t_frame; i++)
    {
        if (frm[i] == -1)
            printf("  |");
        else
            printf("%2d|", frm[i]);
    }
    printf("]");
    if (p_f == 1)
        printf(" F");
    printf("\n");
}

// Function do page replacement using First in First Out approach
int fifo(int t_frame, int numblen, int num[], int frm[], char *dprint)
{
    int fifoind = 0;
    int mss = 0;
    int complt = 0;

    for (int i = 0; i < t_frame; i++)
        frm[i] = -1;

    for (int i = 0; i < numblen; i++)
    {
        if (dprint == NULL)
            printf("%2d: ", num[i]);

        int p_f = 1;
        int indx = -1;
        for (int j = 0; j < t_frame; j++)
        {
            if (frm[j] == -1)
            {
                indx = j;
                p_f = 2;
                break;
            }
            if (frm[j] == num[i])
            {
                p_f = 0;
                break;
            }
        }
        if (dprint == NULL)
            print_answer(frm, t_frame, p_f, i);

        if (p_f == 1)
        {
            frm[fifoind] = num[i];
            fifoind = (fifoind + 1) % t_frame;
            mss++;
        }
        else if (p_f == 2)
        {
            frm[indx] = num[i];
        }
    }
    return mss;
}

// Function do page replacement using Least Recently Used approach
int lru(int t_frame, int numblen, int num[], int frm[], char *dprint)
{
    for (int i = 0; i < t_frame; i++)
        frm[i] = -1;
    int last_used[1000];
    int mss = 0;
    for (int i = 0; i < t_frame; i++)
        last_used[i] = 0;

    int time = 1;

    for (int i = 0; i < numblen; i++)
    {
        if (dprint == NULL)
            printf("%2d: ", num[i]);
        // Check if page fault occurs
        int p_f = 1;
        int indx = -1;
        for (int j = 0; j < t_frame; j++)
        {
            if (frm[j] == -1)
            {
                indx = j;
                p_f = 2;
                break;
            }
            if (frm[j] == num[i])
            {
                p_f = 0;
                last_used[j] = time;
                time++;
                break;
            }
        }
        if (dprint == NULL)
            print_answer(frm, t_frame, p_f, i);

        if (p_f == 1)
        {
            int mini_ind = 0;
            for (int j = 1; j < t_frame; j++)
            {
                // printf("%d  %d\n", last_used[j], frm[mini_ind]);
                if (last_used[j] < last_used[mini_ind])
                {
                    mini_ind = j;
                }
            }
            // printf("%d  %d\n", num[mini_ind], frm[mini_ind]);
            frm[mini_ind] = num[i];
            last_used[mini_ind] = time;
            time++;
            mss++;
        }
        else if (p_f == 2)
        {
            frm[indx] = num[i];
        }
    }

    return mss;
}

// Function do page replacement using Optimal approach
int opt(int t_frame, int numblen, int num[], int frm[], char *dprint)
{
    for (int i = 0; i < t_frame; i++)
        frm[i] = -1;
    int mss = 0;
    for (int i = 0; i < numblen; i++)
    {
        if (dprint == NULL)
            printf("%2d: ", num[i]);
        int p_f = 1, indx = -1;
        for (int j = 0; j < t_frame; j++)
        {
            if (frm[j] == -1)
            {
                indx = j;
                p_f = 2;
                break;
            }
            if (frm[j] == num[i])
            {
                p_f = 0;
                break;
            }
        }
        if (dprint == NULL)
            print_answer(frm, t_frame, p_f, i);

        if (p_f == 1)
        {
            int done = 0;
            mss++;
            for (int j = 0; j < t_frame; j++)
            {
                if (frm[j] == -1)
                {
                    frm[j] = num[i];
                    done = 1;
                    break;
                }
            }
            int max_ind = -1;
            int max_dist = -1;
            if (done)
                continue;
            for (int j = 0; j < t_frame; j++)
            {
                int distance = numblen;
                for (int k = i + 1; k < numblen; k++)
                {
                    if (frm[j] == num[k])
                    {
                        distance = k - i;
                        break;
                    }
                }
                if (distance > max_dist)
                {
                    max_dist = distance;
                    max_ind = j;
                }
            }
            frm[max_ind] = num[i];
        }
        else if (p_f == 2)
        {
            frm[indx] = num[i];
        }
    }

    return mss;
}
