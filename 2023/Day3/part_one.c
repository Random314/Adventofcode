#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEBUG_CODE 0

#define DAY 3
#define PART 1

struct symbol_list
{
    int position;
    int row;

    // Debug
    char symbol;

    struct symbol_list *next;
    struct symbol_list *previous;
};

struct digit_list
{
    int position_start;
    int position_end;
    int row;

    int digit[3];

    struct digit_list *next;
    struct digit_list *previous;
};

struct symbol_list *symbol_head = NULL;
struct symbol_list *symbol_end = NULL;
struct symbol_list *symbol_current = NULL;

struct digit_list *digit_head = NULL;
struct digit_list *digit_end = NULL;
struct digit_list *digit_current = NULL;

void insertNode(struct symbol_list *symbol_node, struct digit_list *digit_node)
{
    if (symbol_node != NULL)
    {
        // Create List if empty
        if (symbol_head == NULL)
        {
            symbol_head = symbol_node;
            symbol_end = symbol_node;
        }
        else
        {
            symbol_node->previous = symbol_end; // Add previous pointer to new node
            symbol_end->next = symbol_node;     // Update next pointer
            symbol_end = symbol_node;           // Update end node
        }
        return; // Only one node should be updated at a time
    }

    if (digit_node != NULL)
    {
        // Create List if empty
        if (digit_head == NULL)
        {
            digit_head = digit_node;
            digit_end = digit_node;
        }
        else
        {
            digit_node->previous = digit_end;
            digit_end->next = digit_node; // Update next pointer
            digit_end = digit_node;       // Update end node
        }
        return; // Only one node should be updated at a time
    }
}

void printSymbolList(void)
{
#if DEBUG_CODE
    struct symbol_list *node = symbol_head;
    while (node != NULL)
    {
        printf("Symbol: %c, Position: %d, Row: %d\n", node->symbol, node->position, node->row);
        if (node == symbol_end)
            node = NULL;
        else
            node = node->next;
    }
#else
    return;
#endif
}

void printDigitList(void)
{
#if DEBUG_CODE
    struct digit_list *node = digit_head;
    while (node != NULL)
    {
        printf("Digits found %d %d %d,", node->digit[0], node->digit[1], node->digit[2]);
        printf(" Pos Start: %d, Pos End: %d, Row: %d\n", node->position_start, node->position_end, node->row);
        if (node == digit_end)
            node = NULL;
        else
            node = node->next;
    }
#else
    return;
#endif
}

void removeDigitNode(struct digit_list *node)
{
    if (node != digit_head && node != digit_end)
    {
        node->previous->next = node->next;
        node->next->previous = node->previous;
        return;
    }

    if (node == digit_head && node == digit_end)
    {
        node->next = NULL; // needed for logic loop, needs to set to NULL
        digit_head = NULL;
        digit_end = NULL;
        return;
    }

    if (node == digit_head)
    {
        digit_head = node->next;
        return;
    }

    if (node == digit_end)
    {
        digit_end = node->previous;
    }
}

int getDigitFromNode(struct digit_list *node)
{
    int return_val = 0;
    for (int i = 0; i < 3; i++)
    {
        int val = node->digit[i];
        if (val != -1)
            return_val = return_val * 10 + val;
        else
            break;
    }
    return return_val;
}

void test_code()
{
    FILE *file_path;
#if DEBUG_CODE
    file_path = fopen("sample.txt", "r");
#else
    file_path = fopen("input.txt", "r");
#endif

    if (file_path == NULL)
        exit(EXIT_FAILURE);

    ssize_t read;
    size_t max_len = 400;
    char *line = malloc(max_len);
    int answer = 0; // Update as needed

    int test_character = 0;
    int test_number = 0;
    int symbol_location[] = {};
    int current_row = 0;

    // Get each line from the file input
    while (fgets(line, max_len, file_path) != NULL)
    {
#if DEBUG_CODE
        printf("\nInput: %s\n", line);
#endif
        // Walk though each char in the array
        // Load all symbols and digits into each array
        for (int i = 0; line[i] != '\0'; i++)
        {
            test_character = line[i];
            test_number = test_character - '0';

            if (0 <= test_number && test_number <= 9)
            {
                struct digit_list *new_node = (struct digit_list *)malloc(sizeof(struct digit_list));
                new_node->position_start = i;
                new_node->row = current_row;

                int number_to_skip = -1;
                // Walk threw see if the next 2 characters are digits
                for (int j = 0; j < 3; j++)
                {
                    int testing = line[i + j] - '0';
                    if (0 <= testing && testing <= 9)
                    {
                        new_node->digit[j] = testing;
                        new_node->position_end = i + j; // Update end postion
                        number_to_skip++;
                    }
                    else
                    {
                        new_node->digit[j] = -1;
                    }
                }
                i += number_to_skip;
                insertNode(NULL, new_node);
            }

            else if (test_character != (int)'.' && test_character != (int)'\n')
            {
                struct symbol_list *new_node = (struct symbol_list *)malloc(sizeof(struct symbol_list));
                new_node->position = i;
                new_node->row = current_row;
                new_node->symbol = test_character;
                insertNode(new_node, NULL);
                continue;
            }
        }

        current_row++;
    }

    // Free file and close
    fclose(file_path);
    if (line)
        free(line);

#if DEBUG_CODE
    printSymbolList();
    printf("\n");
    printDigitList();
    printf("\n");
#endif

    symbol_current = symbol_head;
    // Walk though each symbol
    while (symbol_current != NULL)
    {
#if DEBUG_CODE
        printf("Checking Symbol %c\n", symbol_current->symbol);
#endif
        digit_current = digit_head;
        // Repeat walking down each digit till we find one where the
        // digit is close to the symbol
        // Reset will occur when the row becomes more than 1 away from the symbol
        // or will reset when get to far from the symbol position
        while (digit_current != NULL)
        {
#if DEBUG_CODE
            printf("Symbol %c row %d, Digit row %d digit: %d\n", symbol_current->symbol, symbol_current->row, digit_current->row, getDigitFromNode(digit_current));
#endif

            // Digit is now to far to be concidered as a canidet
            // If the digits row is no longer elgible for any symbol canidets remove it
            if (symbol_current->row > digit_current->row + 1)
            {
                removeDigitNode(digit_current);
            }
            // If the current digits row now exceeds any symbol canidets
            else if (symbol_current->row < digit_current->row - 1)
            {
                break;
            }
            // Now check if the digit position is within +-1 from symbol position
            else if (digit_current->position_start - 1 <= symbol_current->position && symbol_current->position <= digit_current->position_end + 1)
            {
                int dig = getDigitFromNode(digit_current);
                answer += dig;
                if (digit_current == digit_end && symbol_current == symbol_end)
                    break;
                removeDigitNode(digit_current);
            }

            if (digit_current == digit_end)
                digit_current = NULL;
            else
                digit_current = digit_current->next;
        }

        // Move to next node
        if (symbol_current == symbol_end)
            symbol_current = NULL;
        else
            symbol_current = symbol_current->next;
    }

    printf("Answer is: %d\n", answer);
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