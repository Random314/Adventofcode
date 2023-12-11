#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define DEBUG_CODE_ON 1

#if DEBUG_CODE_ON
#define QUESTION_ANSWER 13
#define CARD_NUMBERS_MAX 5
#define ELF_NUMBERS_MAX 8
#else
#define QUESTION_ANSWER -1
#define CARD_NUMBERS_MAX 10
#define ELF_NUMBERS_MAX 25
#endif

#define DAY 4
#define PART 1

#define TRACK_CARD 0
#define TRACK_CARD_NUM 1
#define TRACK_ELF_NUM 2

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

    int answer = 0; // Update as needed

    int cards_list_of_numbers[CARD_NUMBERS_MAX] = {0};
    int elfs_list_of_numbers[ELF_NUMBERS_MAX] = {0};
    int track_card_pos = 0;
    int track_elf_pos = 0;

    int track_card_number = 0;
    int track_digit = 0;
    int tracking_what = TRACK_CARD;

    // Trackers for filling out digits
    int insert_digit = 0;
    int j = 0; // Track current postion to add to line loop

    // Tracking for point
    int current_points = 0;

    // Get each line from the file input
    while (fgets(line, max_len, file_path) != NULL)
    {
#if DEBUG_CODE_ON
        printf("\nInput: %s\n", line);
#endif
        tracking_what = 0;
        track_card_number = 0;
        track_elf_pos = 0;
        track_card_pos = 0;

        memset(elfs_list_of_numbers, 0, sizeof(elfs_list_of_numbers));
        memset(cards_list_of_numbers, 0, sizeof(cards_list_of_numbers));

        for (int i = 0; line[i] != '\0'; i++)
        {
            if (!isdigit(line[i]) && !(line[i] == ':' || line[i] == '|'))
            {
                continue;
            }
            if (line[i] == ':')
            {
                tracking_what = TRACK_CARD_NUM;
#if DEBUG_CODE_ON
                printf("Switching to track Card numbers\n");
                continue;
#endif
            }
            if (line[i] == '|')
            {
                tracking_what = TRACK_ELF_NUM;
#if DEBUG_CODE_ON
                printf("Switching to track Elf numbers\n");
                continue;
#endif
            }

            // Fill out each card#, card array and elf array
            for (j = 0; isdigit(line[i + j]); j++)
            {
                insert_digit = line[i + j] - '0';
                if (tracking_what == TRACK_ELF_NUM)
                {
                    elfs_list_of_numbers[track_elf_pos] = insert_digit + elfs_list_of_numbers[track_elf_pos] * 10;
                }
                else if (tracking_what == TRACK_CARD_NUM)
                {
                    cards_list_of_numbers[track_card_pos] = insert_digit + cards_list_of_numbers[track_card_pos] * 10;
                }
                else
                {
                    track_card_number = insert_digit + track_card_number * 10;
                }
            }

            i += j - 1; // Move tracking forward
            if (tracking_what == TRACK_ELF_NUM)
            {
                track_elf_pos++;
            }
            else if (tracking_what == TRACK_CARD_NUM)
            {
                track_card_pos++;
            }
        }

// Check if elfs numbers match card and count them
#if DEBUG_CODE_ON
        printf("Card %d matching numbers: ", track_card_number);
#endif
        current_points = 0;
        for (int c = 0; c < CARD_NUMBERS_MAX; c++)
        {
            for (int e = 0; e < ELF_NUMBERS_MAX; e++)
            {
                if (cards_list_of_numbers[c] == elfs_list_of_numbers[e])
                {
                    if (current_points == 0)
                    {
                        current_points++;
                    }
                    else
                    {
                        current_points = current_points * 2;
                    }
#if DEBUG_CODE_ON
                    printf(" %d", elfs_list_of_numbers[e]);
#endif
                    e = ELF_NUMBERS_MAX;
                }
            }
        }
        answer += current_points;
#if DEBUG_CODE_ON
        printf("\n");
#endif
    }

    printf("My answer is: %d should be %d\n", answer, QUESTION_ANSWER);

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