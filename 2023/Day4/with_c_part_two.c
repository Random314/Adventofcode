#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define DEBUG_CODE_ON 0

#if DEBUG_CODE_ON
#define QUESTION_ANSWER 30
#define CARD_NUMBERS_MAX 5
#define ELF_NUMBERS_MAX 8

#define MAX_TOTAL_BASE_CARDS 6
#else
#define QUESTION_ANSWER 5037841
#define CARD_NUMBERS_MAX 10
#define ELF_NUMBERS_MAX 25

#define MAX_TOTAL_BASE_CARDS 223
#endif

#define DAY 4
#define PART 2

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
    int track_card_number_pos = 0;
    int track_elf_number_pos = 0;

    int track_card_number = 0;
    int track_digit = 0;
    int tracking_what = TRACK_CARD;

    // Trackers for filling out digits
    int insert_digit = 0;
    int j = 0; // Track current postion to add to line loop

    // Tracking cards
    int track_matches = 0;
    int track_cards_pos = 0;
    int track_cards[MAX_TOTAL_BASE_CARDS] = {0};

    // Get each line from the file input
    while (fgets(line, max_len, file_path) != NULL)
    {
#if DEBUG_CODE_ON
        printf("\nInput: %s\n", line);
#endif
        tracking_what = 0;
        track_card_number = 0;

        track_card_number_pos = 0;
        track_elf_number_pos = 0;

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
                continue;
            }
            if (line[i] == '|')
            {
                tracking_what = TRACK_ELF_NUM;
                continue;
            }

            // Fill out each card#, card array and elf array
            for (j = 0; isdigit(line[i + j]); j++)
            {
                insert_digit = line[i + j] - '0';
                if (tracking_what == TRACK_ELF_NUM)
                {
                    elfs_list_of_numbers[track_elf_number_pos] = insert_digit + elfs_list_of_numbers[track_elf_number_pos] * 10;
                }
                else if (tracking_what == TRACK_CARD_NUM)
                {
                    cards_list_of_numbers[track_card_number_pos] = insert_digit + cards_list_of_numbers[track_card_number_pos] * 10;
                }
                else
                {
                    track_card_number = insert_digit + track_card_number * 10;
                }
            }

            i += j - 1; // Move tracking forward
            if (tracking_what == TRACK_ELF_NUM)
            {
                track_elf_number_pos++;
            }
            else if (tracking_what == TRACK_CARD_NUM)
            {
                track_card_number_pos++;
            }
        }

// Check if elfs numbers match card and count them
#if DEBUG_CODE_ON
        printf("Card %d matching numbers: ", track_card_number);
#endif
        track_matches = 0;
        for (int c = 0; c < CARD_NUMBERS_MAX; c++)
        {
            for (int e = 0; e < ELF_NUMBERS_MAX; e++)
            {
                if (cards_list_of_numbers[c] == elfs_list_of_numbers[e])
                {
                    track_matches++;
                    e = ELF_NUMBERS_MAX;
                }
            }
        }

        // Need to skip no winners = no new cards being given
        if (track_matches == 0)
        {
#if DEBUG_CODE_ON
            printf("Not wining card\n");
#endif
            track_cards_pos++;
            continue;
        }

        // Track all new copies being added due to winners
        track_cards_pos++;
        for (int m = 0; m < track_matches; m++)
        {
            if (track_cards_pos + m > MAX_TOTAL_BASE_CARDS)
            {
#if DEBUG_CODE_ON
                printf("Max size exceeded\n");
#endif
                break;
            }
            track_cards[track_cards_pos + m] += track_cards[track_cards_pos - 1] + 1;
        }
    }

    // Add up all new copies
    for (int a = 0; a < MAX_TOTAL_BASE_CARDS; a++)
    {
        answer = track_cards[a] + answer + 1;
    }

    printf("\nMy answer is: %d should be %d\n", answer, QUESTION_ANSWER);

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