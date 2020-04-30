#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define NUM_ENTRIES 1000000
#define LINE_LENGTH 2000

int NUM_THREADS;
char entries[NUM_ENTRIES][LINE_LENGTH];

int calc_difference(char* line1, char* line2, int line1_length, int line2_length);
void read_file();


void main(int argc, char* argv[])
{
    
}

int calc_difference(char* line1, char* line2, int line1_length, int line2_length)
{
    long sum1 = 0;
    long sum2 = 0;

    for(int i = 0; i < line1_length; i++)
        sum1 += line1[i];

    for(int i = 0; i < line2_length; i++)
        sum2 += line2[i];

    // printf("%ld - %ld\n", sum1, sum2);

    return (int)(sum1 - sum2);
}

void read_file()
[
    FILE* fp = fopen(filename, "r");
    if(fp == NULL)
    {
        perror("Unable to open file!");
        exit(1);
    }

    char line[LINE_LENGTH];

    int i = 0;
    while(fgets(line, LINE_LENGTH, fp) != NULL && i < NUM_ENTRIES)
    {
        strcpy(entries[i], line);
        i++;
    }

    // int line1_counter = 0;
    // int line2_counter = 1;
    // char line1_array[1000];
    // char line2_array[1000];
    // char* line1 = "";
    // char* line2 = fgets(line2_array, 1000, fp);
    // while(line2 != NULL)
    // {
    //     printf("Lines %d-%d: %d\n", line1_counter, line2_counter, calc_difference(line1, line2, strlen(line1), strlen(line2)));
    //     line1_counter++;
    //     line2_counter++;
    //     line1 = strcpy(line1_array, line2);
    //     line2 = fgets(line2_array, 1000, fp);
    // }

    fclose(fp);
]