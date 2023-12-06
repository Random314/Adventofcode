#define _GNU_SORUCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset */

int main(void)
{
    FILE * file_path;
    file_path = fopen("part_one_input.txt", "r");
    //file_path = fopen("sample.txt", "r");

    if (file_path == NULL)
        exit(EXIT_FAILURE);
    
    ssize_t read;
    size_t max_len = 100;
    char* line = malloc(max_len);

    int track_count = 0;
    int num = -1;
    // Get each line and add the value from the first number found append the last number and add to total count
    while (fgets(line, max_len, file_path) != NULL)
    {
        printf("\nLine %s\n", line);
        int first = -1;
        int second = 0;
        for (int i=0; line[i] != '\0'; i++)
        {
            // Number check
            num = line[i] - '0';

            // Check if it is a character if so continue
            if (0 > num || num > 9)
            {
                // Spelled out check
                if (strncmp(&line[i], "one", 3) == 0)
                {
                    num = 1;
                }
                else if (strncmp(&line[i], "two", 3) == 0)
                {
                    num = 2;
                }
                else if (strncmp(&line[i], "three", 5) == 0)
                {
                    num = 3;
                }
                else if (strncmp(&line[i], "four", 4) == 0)
                {
                    num = 4;
                }
                else if (strncmp(&line[i], "five", 4) == 0)
                {
                    num = 5;
                }
                else if (strncmp(&line[i], "six", 3) == 0)
                {
                    num = 6;
                }
                else if (strncmp(&line[i], "seven", 5) == 0)
                {
                    num = 7;
                }
                else if (strncmp(&line[i], "eight", 5) == 0)
                {
                    num = 8;
                }
                else if (strncmp(&line[i], "nine", 4) == 0)
                {
                    num = 9;
                }
                else
                {
                    continue;
                }
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
        num = 0;
        memset(line, 0, sizeof(line));
        printf("Found %d and %d\n", first, second);
        track_count += (first + second);
    }

    printf("Count is: %d\n", track_count);

    fclose(file_path);
    if(line)
        free(line);
    exit(EXIT_SUCCESS);


}