#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // Needed for large numbers
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <inttypes.h>

#define DEBUG_CODE_ON 1

#if DEBUG_CODE_ON
#define QUESTION_ANSWER 35
#define MAX_SEED 4
#else
#define QUESTION_ANSWER 111627841
#define MAX_SEED 20
#endif

#define DAY 5
#define PART 1

#define STATE_SEED 0
#define STATE_SOIL 1
#define STATE_FERTILIZER 2
#define STATE_WATER 3
#define STATE_LIGHT 4
#define STATE_TEMPERATURE 5
#define STATE_HUMIDITY 6
#define STATE_LOCATION 7

struct seed_tracker_s
{
    int value_set[STATE_LOCATION + 1];
    uint64_t stored_value[STATE_LOCATION + 1];
};

struct seed_tracker_s seed_tracker[MAX_SEED] = {0};

void printSeedTracker()
{
    for (int i = 0; i < MAX_SEED; i++)
    {
        printf("Seed ID: %lld\n", seed_tracker[i].stored_value[STATE_SEED]);
    }
}

uint64_t getMapedValue(uint64_t dest_range_start, uint64_t source_range_start, uint64_t range, uint64_t test)
{
    if (source_range_start <= test && test <= source_range_start + range)
    {
        if (source_range_start < test)
            return (test - source_range_start) + dest_range_start;
        else
            return (source_range_start - test) + dest_range_start;
    }
    return test;
}

uint64_t getDigit(int *t, char *line)
{
    uint64_t digit = 0;
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
    char *line = malloc(max_len);
    char *seed_line = malloc(max_len);

    uint64_t answer = 0;

    // this will the first line which is the seed line.
    fgets(seed_line, max_len, file_path);
    uint64_t track_digit = 0;
    int store_value = 0;
    int track_seed_pos = 0;
    for (int s = 0; seed_line[s] != '\0'; s++)
    {
        // Update digit as we step though the line
        if (isdigit(seed_line[s]))
        {
            track_digit = (seed_line[s] - '0') + track_digit * 10;
            continue;
        }

        // If we reach a blank updated the current seed we are tracking
        if (seed_line[s] == ' ' && store_value == 1)
        {
            seed_tracker[track_seed_pos].stored_value[STATE_SEED] = track_digit;
            seed_tracker[track_seed_pos].value_set[STATE_SEED] = 1;
            track_seed_pos++;
            track_digit = 0;
            continue;
        }

        // Reached the first blank before seed numbers
        if (seed_line[s] == ' ' && store_value == 0)
        {
            store_value = 1;
        }
    }
    seed_tracker[track_seed_pos].stored_value[STATE_SEED] = track_digit; // Store final seed
    seed_tracker[track_seed_pos].value_set[STATE_SEED] = 1;

#if DEBUG_CODE_ON
    printf("\nSeed Line input: %s", seed_line);
    printSeedTracker();
#endif

    if (seed_line)
        free(seed_line);

    int i;
    int store_value_as = STATE_SEED;
    while (fgets(line, max_len, file_path) != NULL)
    {
        uint64_t first_digit = 0;
        uint64_t second_digit = 0;
        uint64_t third_digit = 0;
        if (isalpha(line[0]))
        {
            if (store_value_as != STATE_SEED)
            {
                // Same pos as previous
                for (i = 0; i < MAX_SEED; i++)
                {
                    if (seed_tracker[i].value_set[store_value_as] == 0)
                    {
                        seed_tracker[i].stored_value[store_value_as] = seed_tracker[i].stored_value[store_value_as - 1];
                    }
                }
            }

            store_value_as++;
            continue;
        }

        if (isdigit(line[0]))
        {
            i = 0;
            first_digit = getDigit(&i, line);
            i++;
            second_digit = getDigit(&i, line);
            i++;
            third_digit = getDigit(&i, line);
        }
        else
        {
            continue;
        }

        for (i = 0; i < MAX_SEED; i++)
        {
            if (seed_tracker[i].value_set[store_value_as] == 1)
                continue;

            uint64_t mapped_value = getMapedValue(first_digit, second_digit, third_digit, seed_tracker[i].stored_value[store_value_as - 1]);
            if (mapped_value != seed_tracker[i].stored_value[store_value_as - 1])
            {
                seed_tracker[i].value_set[store_value_as] = 1;
                seed_tracker[i].stored_value[store_value_as] = mapped_value;
            }
        }
    }

    // Set final seed values
    for (i = 0; i < MAX_SEED; i++)
    {
        if (seed_tracker[i].value_set[store_value_as] == 0)
        {
            seed_tracker[i].stored_value[store_value_as] = seed_tracker[i].stored_value[store_value_as - 1];
        }
    }

    answer = seed_tracker[0].stored_value[STATE_LOCATION];
    for (i = 0; i < MAX_SEED; i++)
    {
        if (seed_tracker[i].stored_value[STATE_LOCATION] < answer)
            answer = seed_tracker[i].stored_value[STATE_LOCATION];
    }

    printf("My answer is: %llu should be %d\n", answer, QUESTION_ANSWER);

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