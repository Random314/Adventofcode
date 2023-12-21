#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // Needed for large numbers
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <inttypes.h>

#define DEBUG_CODE_ON 0

#if DEBUG_CODE_ON
#define QUESTION_ANSWER 46
#else
#define QUESTION_ANSWER 69323688
#endif

#define DAY 5
#define PART 2

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
    uint64_t start_range;
    uint64_t range_value;

    struct seed_tracker_s *next_node;
};

void addTracker(struct seed_tracker_s *map_node, struct seed_tracker_s **map_head)
{
    map_node->next_node = NULL;
    if (*map_head == NULL)
    {
        (*map_head) = map_node;
        return;
    }
    struct seed_tracker_s *track = *map_head;
    while (track->next_node != NULL)
    {
        track = track->next_node;
    }

    track->next_node = map_node;
}

void removeNode(struct seed_tracker_s *node, struct seed_tracker_s **map_head)
{
    struct seed_tracker_s *track;
    track = *map_head;
    if (*map_head == node)
    {
        if ((*map_head)->next_node == NULL)
        {
            (*map_head) = NULL;
            node = NULL;
        }
        else
        {
            (*map_head) = (*map_head)->next_node;
        }
        return;
    }

    while (track->next_node != node)
    {
        track = track->next_node;
    }
    track->next_node = track->next_node->next_node;
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

void printCurrentMap(struct seed_tracker_s *start)
{
    struct seed_tracker_s *next;
    next = start;
    while (next != NULL)
    {
        printf("Node map start: %llu, range %llu\n", next->start_range, next->range_value);
        next = next->next_node;
    }
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

    size_t max_len = 400;
    char *line = malloc(max_len);
    char *seed_line = malloc(max_len);

    uint64_t answer = UINT64_MAX;

    // this will the first line which is the seed line.
    fgets(seed_line, max_len, file_path);

    // Seed details
    uint64_t seed_start = 0;
    uint64_t seed_range = 0;
    uint64_t seed_end = 0;

    // Map to details
    uint64_t map_next_start = 0;
    uint64_t map_next_end = 0;

    // Map details
    uint64_t map_start = 0;
    uint64_t map_range = 0;
    uint64_t map_end = 0;

    struct seed_tracker_s *seed_head = NULL;
    struct seed_tracker_s *next_map_head = NULL;

    // Get all seed and ranges
    for (int s = 0; seed_line[s] != '\0'; s++)
    {
        // Get to first seed and range
        while (!isdigit(seed_line[s]))
        {
            s++;
            continue;
        }
        seed_start = getDigit(&s, seed_line);
        s++;
        seed_range = getDigit(&s, seed_line);
        struct seed_tracker_s *seed = (struct seed_tracker_s *)malloc(sizeof(struct seed_tracker_s));
        seed->start_range = seed_start;
        seed->range_value = seed_range;
        addTracker(seed, &seed_head);
    }

#if DEBUG_CODE_ON
    printCurrentMap(seed_head);
#endif

    fgets(line, max_len, file_path); // Skip blank line after
    fgets(line, max_len, file_path); // Skip line "seed-to-soil map:"

    struct seed_tracker_s *seed_node;

    while (fgets(line, max_len, file_path) != NULL)
    {
        // Move pointer forward as we progress down the file
        if (isalpha(line[0]))
        {
            if (seed_head == NULL)
            {
                seed_head = next_map_head;
                next_map_head = NULL;
                continue;
            }
            struct seed_tracker_s *track = seed_head;
            while (track->next_node != NULL)
            {
                track = track->next_node;
            }
            track->next_node = next_map_head;
            next_map_head = NULL;
            continue;
        }

        if (isdigit(line[0]))
        {
            int pos = 0;
            map_next_start = getDigit(&pos, line);
            pos++;
            map_start = getDigit(&pos, line);
            pos++;
            map_range = getDigit(&pos, line);
            map_end = map_start + map_range - 1;
            map_next_end = map_next_start + map_range - 1;
        }
        else // Cover blank line
        {
            continue;
        }

        seed_node = seed_head;
        while (seed_node != NULL)
        {
            seed_start = seed_node->start_range;
            seed_range = seed_node->range_value;
            seed_end = seed_start + seed_range - 1;

            // Out side range dont care about them for now
            if (seed_end <= map_start || map_end < seed_start)
            {
                seed_node = seed_node->next_node;
                continue;
            }

            if (seed_start < map_start)
            {
                // Left side update
                seed_node->range_value = map_start - seed_start;

                // If the seed falls in the map range
                if (seed_end <= map_end)
                {
                    // create new middle
                    struct seed_tracker_s *mid_node = (struct seed_tracker_s *)malloc(sizeof(struct seed_tracker_s));

                    mid_node->start_range = map_next_start;
                    uint64_t dif = (map_start - seed_start);
                    mid_node->range_value = seed_range - dif;
                    addTracker(mid_node, &next_map_head);
                }
                else
                {
                    // create new middle node based on map
                    struct seed_tracker_s *mid_node = (struct seed_tracker_s *)malloc(sizeof(struct seed_tracker_s));

                    mid_node->start_range = map_next_start;
                    mid_node->range_value = map_range;
                    addTracker(mid_node, &next_map_head);

                    // Create the right side
                    struct seed_tracker_s *right_node = (struct seed_tracker_s *)malloc(sizeof(struct seed_tracker_s));
                    right_node->start_range = map_end;
                    right_node->range_value = seed_end - map_end;
                    addTracker(right_node, &seed_head);
                }
            }
            // Left side is inside mapping
            else
            {
                // right side is in side
                if (seed_end <= map_end)
                {
                    struct seed_tracker_s *map_node = (struct seed_tracker_s *)malloc(sizeof(struct seed_tracker_s));
                    map_node->start_range = seed_start - map_start + map_next_start;
                    map_node->range_value = seed_range;
                    addTracker(map_node, &next_map_head);
                }
                else
                {
                    // Create new left node
                    struct seed_tracker_s *mid_map_node = (struct seed_tracker_s *)malloc(sizeof(struct seed_tracker_s));
                    mid_map_node->start_range = seed_start - map_start + map_next_start;
                    mid_map_node->range_value = map_end - seed_start;
                    addTracker(mid_map_node, &next_map_head);

                    // Create new right node
                    struct seed_tracker_s *right_map_node = (struct seed_tracker_s *)malloc(sizeof(struct seed_tracker_s));
                    right_map_node->start_range = map_end + 1;
                    right_map_node->range_value = seed_end - (map_end);
                    addTracker(right_map_node, &seed_head);
                }
                // Remove original (node No longer needed)
                struct seed_tracker_s *temp_node;
                temp_node = seed_node;
                seed_node = seed_node->next_node; // Moves node forward
                removeNode(temp_node, &seed_head);
                free(temp_node);
            }
        }
    }

    seed_node = seed_head;
    while (seed_node != NULL)
    {
        if (seed_node->start_range < answer)
        {
            answer = seed_node->start_range;
        }
        seed_node = seed_node->next_node;
    }

    seed_node = next_map_head;
    while (seed_node != NULL)
    {
        if (seed_node->start_range < answer)
        {
            answer = seed_node->start_range;
        }
        seed_node = seed_node->next_node;
    }

    printf("My answer is: %llu should be %d\n", answer, QUESTION_ANSWER);

    // Free file and close
    fclose(file_path);
    if (line)
        free(line);
    if (seed_line)
        free(seed_line);
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