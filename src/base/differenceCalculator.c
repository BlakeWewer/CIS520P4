#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int calc_difference(char* line1, char* line2, int line1_length, int line2_length);
void read_file();

void main()
{
    // char* example_line1 = "abcd";
    // char* example_line2 = "efgh";

    read_file("test.txt");

    // printf(calc_difference(example_line1, example_line2, strlen(example_line1), strlen(example_line2)), "%d\n");
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

void read_file(char* filename)
{
    FILE* fp = fopen(filename, "r");
    if(fp == NULL)
    {
        perror("Unable to open file!");
        exit(1);
    }

    int line1_counter = 0;
    int line2_counter = 1;
    char line1_array[1000];
    char line2_array[1000];
    char* line1 = "";
    char* line2 = fgets(line2_array, 1000, fp);
    while(line2 != NULL)
    {
        printf("Lines %d-%d: %d\n", line1_counter, line2_counter, calc_difference(line1, line2, strlen(line1), strlen(line2)));
        line1_counter++;
        line2_counter++;
        line1 = strcpy(line1_array, line2);
        line2 = fgets(line2_array, 1000, fp);
    }

    fclose(fp);
}