#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEBUG_CODE 0
#define PERFORMANCE_CODE 1

int execute = PERFORMANCE_CODE;
char *color_name[] = {"red", "green", "blue"};
char color_name_size[] = {3, 5, 4};
int number_of_colors[] = {12, 13, 14};

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

    int track_game_total_count = 0;
    int failing = 0;
    // Get each line from the file input
    while (fgets(line, max_len, file_path) != NULL)
    {
        if (execute == DEBUG_CODE)
            printf("\nInput: %s", line);

        int game = 0;
        int new_number = 0;
        int track_number = 0;

        failing = 0;

        // Walk though each char in the array
        for (int i = 0; line[i] != '\0'; i++)
        {
            new_number = line[i] - '0';
            // If num is inbetween 0 or 9 it is most likely a number
            if (0 <= new_number && new_number <= 9)
            {
                track_number = (track_number * 10) + new_number;
                continue;
            }

            // Reset tracking number and set the game value to this
            if (strncmp(&line[i], ":", 1) == 0)
            {
                game = track_number;
                track_number = 0;
                continue;
            }

            // Check to see if a color, and if so, set the current tracked number to that value
            for (int j = 0; j < 3; j++)
            {
                // Check if string matches our color name
                if (strncmp(&line[i], color_name[j], color_name_size[j]) == 0)
                {
                    // If the number exceeds the expected tracked
                    if (track_number > number_of_colors[j])
                    {
                        if (execute == DEBUG_CODE)
                            printf("Game %d to fail\nReason: color: %s, only has %d, game has %d\n", game, color_name[j], number_of_colors[j], track_number);
                        failing = 1;
                        break;
                    }
                    track_number = 0;
                }
            }
            if (failing)
                break;
        }
        if (!failing)
            track_game_total_count += game;
    }

    printf("Total count is: %d\n", track_game_total_count);

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
    printf("Elapsed time Day 2, part 1: %lu ns\n", end.tv_nsec - start.tv_nsec);

    exit(EXIT_SUCCESS);
}