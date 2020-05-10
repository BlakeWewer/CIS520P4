#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 4
#define NUM_LINES 20

char *lines[NUM_LINES];
int line_diffs[NUM_LINES - 1];

void read_file();
void line_diff(size_t my_id);
int line_sum(size_t line);
void free_lines();
void print_diffs();

int main(void)
{
    omp_set_num_threads(NUM_THREADS);

    read_file();

    #pragma omp parallel
    {
        line_diff(omp_get_thread_num());
    }

    free_lines();
    print_diffs();
}

void read_file()
{
    FILE *fp = fopen("test.txt", "r");

    if (fp == NULL)
    {
        perror("[openmp/ERROR] [read_file/fopen]");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < NUM_LINES; i++)
    {
        char *line = NULL;
        size_t len;

        if (getline(&line, &len, fp) == -1)
        {
            perror("[openmp/ERROR] [read_file/getline]");
            free(line);
            exit(EXIT_FAILURE);
        }

        lines[i] = line;
    }

    fclose(fp);
}

void line_diff(size_t id)
{
    size_t start, end, i;
    int previous_sum, sum, diff;

    #pragma omp private(id, start, end, i, previous_sum, sum, diff)
    {
        start = id * (NUM_LINES / NUM_THREADS);
        end = start + (NUM_LINES / NUM_THREADS);
        previous_sum = line_sum(start);

        printf("id = %zu | start = %zu | end = %zu\n", id, start, end);

        for (i = start + 1; i < end; i++)
        {
            sum = line_sum(i);
            diff = previous_sum - sum;

            line_diffs[i - 1] = diff;

            previous_sum = sum;
        }
    }
}

int line_sum(size_t line_num)
{
    char *line = lines[line_num];
    int result = 0;

    for (size_t i = 0; i < strlen(line); i++)
    {
        result += (int)line[i];
    }

    return result;
}

void free_lines()
{
    for (size_t i = 0; i < NUM_LINES; i++)
    {
        free(lines[i]);
    }
}

void print_diffs()
{
    for (size_t i = 0; i < NUM_LINES - 1; i++)
    {
        printf("%zu-%zu: %d\n", i, i + 1, line_diffs[i]);
    }
}