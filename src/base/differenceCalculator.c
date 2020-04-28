#include <stdio.h>
#include <stdlib.h>


int calc_difference(char* line1, char* line2, int line1_length, int line2_length);
void read_file();

void main()
{
    // char* example_line1 = "abcd";
    // char* example_line2 = "efgh";

    read_file();

    // printf(calc_difference(example_line1, example_line2, strlen(example_line1), strlen(example_line2)), "%d\n");
}

int calc_difference(char* line1, char* line2, int line1_length, int line2_length)
{
    int sum1 = 0;
    int sum2 = 0;

    for(int i = 0; i < line1_length; i++)
        sum1 += line1[i];

    for(int i = 0; i < line2_length; i++)
        sum2 += line2[i];

    return sum1 - sum2;
}

void read_file()
{
    FILE* fp = fopen("test.txt", "r");
    if(fp == NULL)
    {
        perror("Unable to open file!");
        exit(1);
    }

    char chunk[128];

    while(fgets(chunk, sizeof(chunk), fp) != NULL)
    {
        fputs(chunk, stdout);
        fputs("|*\n", stdout);
    }

    fclose(fp);
}