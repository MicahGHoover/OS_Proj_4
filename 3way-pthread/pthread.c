//Feel free to add more libraries if needed
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// Structure to hold information for each thread
struct ThreadData {
    char *line;
    int max_ascii;
};

// Potential Function to find maximum ASCII value in a line
// Pass this function into the pthread create in the main
void *find_max_ascii(void *arg) {
    // Cast the argument to the appropriate type
    struct ThreadData *data = (struct ThreadData *)arg;

    // Initialize max_ascii to 0
    data->max_ascii = 0;
    
    // Loop through each character in the line until the null terminator ('\0') is encountered
    for (int i = 0; data->line[i] != '\0'; i++) {
        // Retrieve the ASCII value of the current character
        int ascii_value = (int)data->line[i];
        
        // If the ASCII value of the current character is greater than the current max_ascii value, update max_ascii
        if (ascii_value > data->max_ascii) {
            data->max_ascii = ascii_value;
        }
    }
    
    // Exit the thread
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

  //DO THINGS

  fclose(file);

  //PROBABLY DO MORE THINGS

  return 0;

}
