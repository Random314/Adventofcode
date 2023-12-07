#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEBUG_CODE 0
#define PERFORMANCE_CODE 1

#define DAY 0 // Update
#define PART 2

int execute = DEBUG_CODE;

void test_code()
{
    FILE *file_path;
    file_path = fopen("input.txt", "r");
    // file_path = fopen("sample_input.txt", "r");

    if (file_path == NULL)
        exit(EXIT_FAILURE);

    ssize_t read;
    size_t max_len = 400;
    char *line = malloc(max_len);
    int answer = 0; // Update as needed

    // Get each line from the file input
    while (fgets(line, max_len, file_path) != NULL)
    {
        if (execute == DEBUG_CODE)
            printf("\nInput: %s\n", line);
    }

    printf("Answer is: %d\n", answer);

    // Free file and close
    fclose(file_path);
    if (line)
        free(line);
}

int main(void)
{
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    test_code();

    clock_gettime(CLOCK_REALTIME, &end);
    printf("Elapsed time Day %d, part %d: %lu ns\n", DAY, PART, end.tv_nsec - start.tv_nsec);

    exit(EXIT_SUCCESS);
}