#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int max_ascii_value(char *line, int nchars) {
    int max = 0;
    for (int i = 0; i < nchars; i++) {
        if ((int)line[i] > max)
            max = (int)line[i];
    }
    return max;
}

int main() {
    int world_rank, world_size; // int for rank and number of processes

    MPI_Init(NULL, NULL); // initialize the MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // get rank of processes
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); // get size of processes

    // Attempt to open the file
    FILE *file = fopen("/homes/dan/625/wiki_dump.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        MPI_Finalize();
        return 1;
    }

    char *line = NULL;
    size_t line_length = 0;
    int line_number = 0;
    int lines_per_process = 0;
    int lines_to_process = 0;

    // Determine the number of lines to process by each process
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    lines_per_process = file_size / world_size;
    lines_to_process = (world_rank < file_size % world_size) ? lines_per_process + 1 : lines_per_process;

    // Move file pointer to the start of the lines for this process
    if (world_rank != 0) {
        int lines_to_skip = (world_rank < file_size % world_size) ? lines_per_process + 1 : lines_per_process;
        for (int j = 0; j < lines_to_skip; j++) {
            getline(&line, &line_length, file);
        }
    }

    // Process lines
    for (int i = 0; i < lines_to_process; i++) {
        getline(&line, &line_length, file);
        int line_length_int = strlen(line);
        int local_max = max_ascii_value(line, line_length_int);

        // Print the result in order with barrier synchronization
        MPI_Barrier(MPI_COMM_WORLD);
        printf("%d: %d\n", line_number, local_max);

        line_number++;
    }

    fclose(file);
    free(line);

    MPI_Finalize(); // finalize and close MPI environment
    return 0;
}
