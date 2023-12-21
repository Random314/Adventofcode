#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>  // isdigit()
#include <stdint.h> // Large int

#define DEBUG_CODE_ON 0

#if DEBUG_CODE_ON
#define QUESTION_ANSWER 71503
#else
#define QUESTION_ANSWER -1
#endif

#define DAY 6
#define PART 2

uint64_t getDigit(int *t, char *line)
{
    uint64_t digit = 0;
    for (; (isdigit(line[*t]) || line[*t] == ' '); (*t)++)
    {
        if (line[*t] == ' ')
            continue;
        digit = (line[*t] - '0') + digit * 10;
    }
    return digit;
}

void test_code()
{
    FILE *file_path;
#if DEBUG_CODE_ON
    file_path = fopen("_sample.txt", "r");
#else
    file_path = fopen("_input.txt", "r");
#endif
    if (file_path == NULL)
        exit(EXIT_FAILURE);

    ssize_t read;
    size_t max_len = 400;
    char *time_line = malloc(max_len);
    char *dist_line = malloc(max_len);

    uint64_t answer = 1;
    uint64_t max_game_time = 0;
    uint64_t game_dist_to_win = 0;

    // Get Time Line
    fgets(time_line, max_len, file_path);
    // Get Distance Line
    fgets(dist_line, max_len, file_path);
#if DEBUG_CODE_ON
    printf("\nTime Input: %s\n", time_line);
    printf("\nDistance Input: %s\n", dist_line);
#endif

    // Get all times
    int pos = 0;
    while (!isdigit(time_line[pos]))
    {
        pos++;
    }
    max_game_time = getDigit(&pos, time_line);

    pos = 0;
    while (!isdigit(dist_line[pos]))
    {
        pos++;
    }
    game_dist_to_win = getDigit(&pos, dist_line);

    int speed, possible, hold_time;
    hold_time = 0;
    possible = 0;
    while (hold_time < max_game_time)
    {
        speed = hold_time;
        if ((max_game_time - speed) * speed > game_dist_to_win)
        {
            possible++;
        }
        hold_time++;
    }
    answer = possible;

    printf("My answer is: %lld should be %d\n", answer, QUESTION_ANSWER);

    // Free file and close
    fclose(file_path);
    if (time_line)
        free(time_line);
    if (dist_line)
        free(dist_line);
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