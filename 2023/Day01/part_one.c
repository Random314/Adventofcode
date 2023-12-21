#define _GNU_SORUCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset */

int main(void)
{
    FILE *file_path;
    file_path = fopen("part_one_input.txt", "r");

    if (file_path == NULL)
        exit(EXIT_FAILURE);

    ssize_t read;
    size_t max_len = 100;
    char *line = malloc(max_len);

    int track_count = 0;
    int num = -1;
    // Get each line and add the value from the first number found append the last number and add to total count
    while (fgets(line, max_len, file_path) != NULL)
    {
        printf("\nLine %s\n", line);
        int first = -1;
        int second = 0;
        // Walk though each char in the array
        for (int i = 0; line[i] != '\0'; i++)
        {
            num = line[i] - '0';
            // If num is less than 0 or greater than 9 it is most likely a character
            if (0 > num || num > 9)
            {
                continue;
            }

            printf("number %d, letter %c\n", num, line[i]);
            // Check if the int is a number and if the number is 0
            if (num != 0)
            {
                // if it is a number check if first has been done and
                if (first == -1)
                {
                    first = num * 10;
                }
                second = num;
            }
        }
        printf("Found %d and %d\n", first, second);
        track_count += (first + second);
    }

    printf("Count is: %d\n", track_count);

    fclose(file_path);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}