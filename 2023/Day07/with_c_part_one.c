#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h> // Large int
#include <ctype.h>  // isdigit()

#define DEBUG_CODE_ON 0

#if DEBUG_CODE_ON
#define QUESTION_ANSWER 6440
#else
#define QUESTION_ANSWER -1
#endif

#define DAY 7
#define PART 1

// 5 of kind, 4 of a kind, full house, 3 of kind, 2 pair, one pair, high card
int hand_type[] = {7, 6, 5, 4, 3, 2, 1};

struct hand_and_bid
{
    int hand[5];
    uint16_t bid;

    uint8_t hand_type;

    struct hand_and_bid *next;
    struct hand_and_bid *previous;
};

struct hand_and_bid *hb_head = NULL;

uint8_t classifyNode(struct hand_and_bid *node)
{
    int matching[5] = {0};
    int match_int[5] = {0};

    for (int i = 0; i < 5; i++)
    {
        match_int[i] = node->hand[i];
    }

    int j = 1;
    for (int i = 0; i < 5; i++)
    {
        if (match_int[i] == 0)
        {
            continue;
        }
        matching[i] = 1;
        j = i + 1;
        while (j < 5)
        {
            if (match_int[i] == match_int[j])
            {
                match_int[j] = 0;
                matching[i]++;
            }
            j++;
        }
    }

    uint8_t classification = 0;
    for (int i = 0; i < 5; i++)
    {
        switch (matching[i])
        {
        case 5:
            classification = 7; // 5 of a kind
            break;
        case 4:
            classification = 6; // 4 of a kind
            break;
        case 3:
            if (classification == 2)
            {
                classification = 5; // Full house
            }
            else
            {
                classification = 4; // 3 of a kind
            }
            break;
        case 2:
            if (classification == 4) // Full House
            {
                classification = 5;
            }
            else if (classification == 2) // Two pair
            {
                classification = 3;
            }
            else
            {
                classification = 2; // Pair default
            }
            break;
        default:
            classification = classification == 0 ? 1 : classification;
            break;
        }
    }
    return classification;
}

void addNode(struct hand_and_bid *node)
{
    struct hand_and_bid *pointer_node;
    pointer_node = hb_head;
    node->hand_type = classifyNode(node);

    if (hb_head == NULL)
    {
        hb_head = node;
        return;
    }
    while (pointer_node->next != NULL && pointer_node->hand_type > node->hand_type)
    {
        pointer_node = pointer_node->next;
    }

    // Insert at end node hand type less thank pointer hand type
    if (pointer_node->next == NULL && pointer_node->hand_type > node->hand_type)
    {
        pointer_node->next = node;
        node->previous = pointer_node;
        return;
    }

    // Check if we found a case we need to insert before
    if (pointer_node->hand_type < node->hand_type)
    {
        // Insert before
        node->next = pointer_node;

        if (pointer_node->previous != NULL)
        {
            node->previous = pointer_node->previous;
            node->previous->next = node;
        }

        pointer_node->previous = node;

        // Update head as we are inserting at the start
        if (pointer_node == hb_head)
            hb_head = node;
        return;
    }

    // Last check they are equal, need to find the higher ranking
    // Compare pointer node while they are equal check to see if new node is greater than current
    while (pointer_node->hand_type == node->hand_type)
    {
        for (int i = 0; i < 5; i++)
        {
            if (pointer_node->hand[i] == node->hand[i])
                continue;
            if (pointer_node->hand[i] > node->hand[i])
            {
                // Move pointer forward
                if (pointer_node->next != NULL)
                    pointer_node = pointer_node->next;
                break;
            }

            // Insert before
            node->next = pointer_node;
            node->previous = pointer_node->previous;
            node->next->previous = node;
            if (node->previous != NULL)
                node->previous->next = node;
            else
                hb_head = node;
            return;
        }

        if (pointer_node->next == NULL && pointer_node->hand_type == node->hand_type)
        {
            pointer_node->next = node;
            node->previous = pointer_node;
            return;
        }
    }

    if (pointer_node->hand_type < node->hand_type)
    {
        node->next = pointer_node;
        node->previous = pointer_node->previous;
        node->next->previous = node;

        if (node->previous != NULL)
            node->previous->next = node;
        else
            hb_head = node;
        return;
    }
    // Insert after
    node->previous = pointer_node;
    node->next = pointer_node->next;
    pointer_node->next = node;
    return;
}

void printNode()
{
    struct hand_and_bid *pointer_node;
    pointer_node = hb_head;

    while (pointer_node != NULL)
    {
        if (pointer_node->previous != NULL)
        {
            int *hand = pointer_node->previous->hand;
            printf("Previous Node Hand: %d %d %d %d %d\n", hand[0], hand[1], hand[2], hand[3], hand[4]);
        }

        int *chand = pointer_node->hand;
        printf("Node Hand: %d %d %d %d %d\n", chand[0], chand[1], chand[2], chand[3], chand[4]);

        if (pointer_node->next != NULL)
        {
            int *nhand = pointer_node->next->hand;
            printf("Next Node Hand: %d %d %d %d %d\n", nhand[0], nhand[1], nhand[2], nhand[3], nhand[4]);
        }
        printf("\n");

        pointer_node = pointer_node->next;
    }
}

void printOrder()
{
    struct hand_and_bid *pointer_node;
    pointer_node = hb_head;

    while (pointer_node != NULL)
    {
        printf("%d(%d) ", pointer_node->bid, pointer_node->hand_type);
        pointer_node = pointer_node->next;
    }
    printf("\n");
}

uint64_t addNodes()
{
    struct hand_and_bid *pointer_node;
    pointer_node = hb_head;

    uint64_t answer = 0;
    uint32_t counter = 1;
    while (pointer_node->next != NULL)
    {
        pointer_node = pointer_node->next;
    }

    while (pointer_node != NULL)
    {
        answer += pointer_node->bid * counter;
        counter++;
        pointer_node = pointer_node->previous;
    }
    return answer;
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

    uint64_t answer = 0;
    hb_head = NULL;
    // Get each line from the file input
    while (fgets(line, max_len, file_path) != NULL)
    {
#if DEBUG_CODE_ON
        printf("\nInput: %s\n", line);
#endif
        int value = 0;
        struct hand_and_bid *node = (struct hand_and_bid *)malloc(sizeof(struct hand_and_bid));
        node->next = NULL;
        node->previous = NULL;
        for (int i = 0; i < 5; i++)
        {
            switch (line[i])
            {
            case 'A':
                value = 14;
                break;
            case 'K':
                value = 13;
                break;
            case 'Q':
                value = 12;
                break;
            case 'J':
                value = 11;
                break;
            case 'T':
                value = 10;
                break;

            default:
                value = line[i] - '0';
                break;
            }
            node->hand[i] = value;
        }
        node->bid = 0;
        for (int i = 6; isdigit(line[i]); i++)
        {
            node->bid = (line[i] - '0') + node->bid * 10;
        }

        addNode(node);
    }

#if DEBUG_CODE_ON
    printNode();
#endif
    // #if DEBUG_CODE_ON
    printOrder();
    // #endif
    answer = addNodes();

    printf("My answer is: %lld should be %d\n", answer, QUESTION_ANSWER);

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