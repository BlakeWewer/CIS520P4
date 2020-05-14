#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define NUM_ENTRIES 1000000
#define LINE_LENGTH 2003

int NUM_THREADS;
char entries[NUM_ENTRIES][LINE_LENGTH];

int results[NUM_ENTRIES];
int local_results[NUM_ENTRIES];

int calc_difference(char* line1, char* line2, int line1_length, int line2_length);
void read_file();


void main(int argc, char* argv[])
{
    struct timeval t1, t2, t3, t4;
    double elapsedTime;

    int numSlots, myVersion = 4;

    int i, rc, numTasks, rank, count, dest, source, tag = 1;
    MPI_Status Status;

    rc = MPI_Init(&argc, &argv);

    if(rc != MPI_SUCCESS)
    {
        printf("\n\nError starting MPI.\n\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    NUM_THREADS = numTasks;
    printf("size = %d - rank = %d\n", numTasks, rank);
    fflush(stdout);

    gettimeofday(&t1, NULL);
	if(rank == 0 ) 
    {
		read_file("/homes/dan/625/wiki_dump.txt");
        // read_file("src/base/test.txt");
	}
	gettimeofday(&t2, NULL);

    MPI_Bcast(entries, NUM_ENTRIES * LINE_LENGTH, MPI_CHAR, 0, MPI_COMM_WORLD);

    gettimeofday(&t3, NULL);

    int line1_counter = -1;
    int line2_counter = 0;
    char line1_array[1000];
    char line2_array[1000];
    char* line1 = "";
    char* line2 = entries[line2_counter];
    while(line2 != NULL)
    {
        //printf("Lines %d-%d: %d\n", line1_counter, line2_counter, calc_difference(line1, line2, strlen(line1), strlen(line2)));
        local_results[line2_counter] = calc_difference(line1, line2, strlen(line1), strlen(line2));
        line1_counter++;
        line2_counter++;
        line1 = entries[line1_counter];
        line2 = entries[line2_counter];
    }

    gettimeofday(&t4, NULL);
    MPI_Reduce(local_results, results, NUM_ENTRIES * LINE_LENGTH, MPI_CHAR, MPI_SUM, 0, MPI_COMM_WORLD);

    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("Time to read file: %f\n", elapsedTime);

	elapsedTime = (t4.tv_sec - t3.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t4.tv_usec - t3.tv_usec) / 1000.0; // us to ms
	printf("Time to calculate difference: %f\n", elapsedTime);

    printf("DATA, %d, %s, %f\n", myVersion, getenv("SLURM_CPUS_ON_NODE"), elapsedTime);


    // if(rank == 0)
    // {
    //     dest = 1;
    //     source = 1;
    //     rc = MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    //     rc = MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
    // }
    // else if (rank == 1)
    // {
    //     dest = 0;
    //     source = 0;
    //     rc = MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    //     rc = MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
    // }

    MPI_Finalize();
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
        perror("Unable to open file!  File is - ");
        exit(1);
    }

    char line[LINE_LENGTH];

    int i = 0;
    while(fgets(line, LINE_LENGTH, fp) != NULL && i < NUM_ENTRIES)
    {
        strcpy(entries[i], line);
        i++;
    }

    fclose(fp);
}