#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> // isdigit()

#define DEBUG_CODE_ON 0

#if DEBUG_CODE_ON
#define QUESTION_ANSWER 288
#define GAMES_PLAYED 3
#else
#define QUESTION_ANSWER 220320
#define GAMES_PLAYED 4
#endif

#define DAY 6
#define PART 1

int max_game_time[GAMES_PLAYED] = {0};
int game_dist_to_win[GAMES_PLAYED] = {0};

int getDigit(int *t, char *line)
{
    int digit = 0;
    for (; isdigit(line[*t]); (*t)++)
    {
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

    int answer = 1;
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
    for (int t = 0; time_line[t] != '\0'; t++)
    {
        if (isdigit(time_line[t]))
        {
            max_game_time[pos] = getDigit(&t, time_line);
            pos++;
        }
    }
    // Get all distances
    pos = 0;
    for (int d = 0; dist_line[d] != '\0'; d++)
    {
        if (isdigit(dist_line[d]))
        {
            game_dist_to_win[pos] = getDigit(&d, dist_line);
            pos++;
        }
    }

    int speed, possible, hold_time;
    for (int game = 0; game < GAMES_PLAYED; game++)
    {
        hold_time = 0;
        possible = 0;
        while (hold_time <= max_game_time[game])
        {
            speed = hold_time;
            if ((max_game_time[game] - speed) * speed > game_dist_to_win[game])
            {
                possible++;
            }
            hold_time++;
        }
        answer = answer * possible;
    }

    printf("My answer is: %d should be %d\n", answer, QUESTION_ANSWER);

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