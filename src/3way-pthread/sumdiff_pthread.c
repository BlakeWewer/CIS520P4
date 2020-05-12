#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 16
#define NUM_LINES 1000000

char *lines[NUM_LINES];
int line_diffs[NUM_LINES - 1];

void read_file();
void line_diff(size_t my_id);
int line_sum(size_t line);
void free_lines();
void print_diffs();

int main(void)
{
    struct timeval t1, t2, t3, t4;
    double elapsed_time;
    int my_version = 3;

    int i, rc;
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    void *status;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Start time of reading file
    gettimeofday(&t1, NULL);

    read_file();

    // End time of reading file
    gettimeofday(&t2, NULL);

    // Start time of parallel code
    gettimeofday(&t3, NULL);

    for (i = 0; i < NUM_THREADS; i++)
    {
        rc = pthread_create(&threads[i], &attr, line_diff, (void *)i);

        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    pthread_attr_destroy(&attr);

    for (i = 0; i < NUM_THREADS; i++)
    {
        rc = pthread_join(threads[i], &status);

        if (rc)
        {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    // End time of parallel code
    gettimeofday(&t4, NULL);

    // Calculate and print time to read file
    elapsed_time = (t2.tv_sec - t1.tv_sec) * 1000.0;
    elapsed_time += (t2.tv_usec - t1.tv_usec) / 1000.0;
    printf("Time to read file: %f\n", elapsed_time);

    // Calculate and print time to calculate line differences
    elapsed_time = (t4.tv_sec - t3.tv_sec) * 1000.0;
    elapsed_time += (t4.tv_usec - t3.tv_usec) / 1000.0;
    printf("Time to calculate difference: %f\n", elapsed_time);

    printf("DATA, %d, %s, %f\n", my_version, getenv("SLURM_CPUS_ON_NODE"), elapsed_time);

    free_lines();
    pthread_exit(NULL);
}

void read_file()
{
    FILE *fp = fopen("/homes/dan/625/wiki_dump.txt", "r");

    if (fp == NULL)
    {
        perror("Failed: ");
        exit(EXIT_FAILURE);
    }

    size_t i;
    for (i = 0; i < NUM_LINES; i++)
    {
        char *line = NULL;
        size_t len;

        if (getline(&line, &len, fp) == -1)
        {
            perror("Failed: ");
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

    pthread_exit(NULL);
}

int line_sum(size_t line_num)
{
    char *line = lines[line_num];
    int result = 0;

    size_t i;
    for (i = 0; i < strlen(line); i++)
    {
        result += (int)line[i];
    }

    return result;
}

void free_lines()
{
    size_t i;
    for (i = 0; i < NUM_LINES; i++)
    {
        free(lines[i]);
    }
}

void print_diffs()
{
    size_t i;
    for (i = 0; i < NUM_LINES - 1; i++)
    {
        printf("%zu-%zu: %d\n", i, i + 1, line_diffs[i]);
    }
}