#include <stdio.h>    // Include standard input/output library
#include <stdlib.h>   // Include standard library
#include <stdint.h>   // Include standard integer types library
#include <string.h>   // Include string manipulation library
#include <sys/stat.h> // for stat function

#define sizeOfStack 10000

int beginFound = 0;

typedef struct
{
    int data[sizeOfStack];
    int top;
} Stack; // defining a stack in C with max size = 100

void initialize(Stack *stack)
{ //  initializing the stack
    stack->top = -1;
}

int isEmpty(Stack *stack)
{ // checking if the stack is empty
    return stack->top == -1;
}

int isFull(Stack *stack)
{ // checking if the stack is full
    return stack->top == sizeOfStack - 1;
}

void push(Stack *stack, int value)
{ // pushing an elememt into the stack
    if (isFull(stack))
    {
        printf("Stack is full\n");
        exit(1);
    }
    stack->data[++stack->top] = value;
}

int pop(Stack *stack)
{ // popping the top element of the stack
    return stack->data[stack->top--];
}

// External declarations
extern int yylex(), yylineno; // External declaration for yylex function, yylineno variable
extern char *yytext;          // External declaration for yytext variable

// Node structure for hash table
struct node // Define a structure named node
{
    char key[200];     // Character array to store key
    int value;         // Integer to store value
    struct node *next; // Pointer to the next node
};

// Hash table and related variables
struct node *hashTable[400], *lastNode[400]; // Array of pointers to nodes for hash table

int numberOfElements[400]; // Array to store number of elements in each hash table slot

// Function to create a new node
struct node *createNode(char *key, int value) // Define function to create a new node
{
    // Allocate memory for a new node
    struct node *temp = (struct node *)malloc(sizeof(struct node));

    // Copy the key to the node
    strcpy(temp->key, key);

    // Set the value of the node
    temp->value = value;

    // Set the next pointer of the node to NULL
    temp->next = NULL;

    // Return the newly created node
    return temp;
}

uint64_t hashFunction(char *key) // Define hashFunction
{
    uint64_t h = 525201411107845655ull; // Initialize hash value

    // Iterate through the characters of the key
    while (*key)
    {
        // Update the hash value
        h ^= *key;
        h *= 0x5bd1e9955bd1e995;
        h ^= h >> 47;
        ++key;
    }

    // Return the final hash value
    return h;
}

// Function to install an identifier into the symbol table
void install_id(char *key, int value, int *listIndex, int *hashIndex) // Define install_id function
{
    uint64_t hash = hashFunction(key); // Calculate hash value for the key
    *hashIndex = hash % 400;           // Calculate hash index

    struct node *itr = hashTable[*hashIndex]; // Initialize iterator to the beginning of linked list at hash index

    int idx = 0; // Initialize index counter

    // Iterate through the linked list
    while (itr != NULL)
    {
        // Check if the key already exists in the hash table
        if (strcmp(itr->key, key) == 0)
        {
            // If found, update the list index and return
            *listIndex = idx;
            return;
        }
        // Move to the next node in the linked list
        itr = itr->next;
        // Increment index counter
        idx++;
    }

    if (beginFound == 1)
    {
        printf("Error: Invalid token. All identifiers used in the program must be declared within a single block, at the beginning of the program \n");
        exit(1);
    }

    // Create a new node for the key-value pair
    struct node *newNode = createNode(key, value);

    // Check if the slot in the hash table is empty
    if (numberOfElements[*hashIndex] == 0)
    {
        // If empty, set the new node as both the first and last node in the slot
        hashTable[*hashIndex] = newNode;
        lastNode[*hashIndex] = newNode;
    }
    else
    {
        // If not empty, append the new node to the end of the linked list
        lastNode[*hashIndex]->next = newNode;

        // Update the last node pointer to the new node
        lastNode[*hashIndex] = newNode;
    }

    // Increment the number of elements in the slot
    numberOfElements[*hashIndex]++;

    // Update the list index to point to the newly added node
    *listIndex = numberOfElements[*hashIndex] - 1;
}

// Function to install a number into the symbol table
void install_num(char *key, int value, int *listIndex, int *hashIndex) // Define install_num function
{
    uint64_t hash = hashFunction(key); // Calculate hash value for the key

    *hashIndex = hash % 400; // Calculate hash index

    struct node *itr = hashTable[*hashIndex]; // Initialize iterator to the beginning of linked list at hash index

    int idx = 0; // Initialize index counter

    // Iterate through the linked list
    while (itr != NULL)
    {
        // Check if the key already exists in the hash table
        if (strcmp(itr->key, key) == 0)
        {
            // If found, update the list index and return
            *listIndex = idx;
            return;
        }

        // Move to the next node in the linked list
        itr = itr->next;

        // Increment index counter
        idx++;
    }

    // Create a new node for the key-value pair
    struct node *newNode = createNode(key, value);

    // Check if the slot in the hash table is empty
    if (numberOfElements[*hashIndex] == 0)
    {
        // If empty, set the new node as both the first and last node in the slot
        hashTable[*hashIndex] = newNode;
        lastNode[*hashIndex] = newNode;
    }
    else
    {
        // If not empty, append the new node to the end of the linked list
        lastNode[*hashIndex]->next = newNode;
        // Update the last node pointer to the new node
        lastNode[*hashIndex] = newNode;
    }

    // Increment the number of elements in the slot
    numberOfElements[*hashIndex]++;

    // Update the list index to point to the newly added node
    *listIndex = numberOfElements[*hashIndex] - 1;
}

// Main function
int main()
{
    // Open output file for writing
    FILE *fout = fopen("result.txt", "w");

    // Initialize number of elements in each hash table slot to 0
    memset(numberOfElements, 0, sizeof(numberOfElements));

    int token, lastLine = 0; // Initialize last line number

    token = yylex(); // Get the first token

    Stack str;                  // declaring the stack
    int beginNo = 0, endNo = 0; //  keeping track of begin and end numbers
    int programEnd = 0;         // to check the end of the program

    initialize(&str); // initializing the stack

    // Write header to output file
    fprintf(fout, "  LINE  TOKEN TYPE         TOKEN SPECIFIER\n  ----  ----------         ---------------\n");

    int tokenCount = 0; // to keep a count of all the tokens

    // Loop through tokens
    while (token)
    {

        if (programEnd == 1)
        {
            printf("error : No code can be written after the end of the program 'END.'\n");
            exit(1);
        }

        if (token == 5)
            programEnd = 1;

        if (token == 3)
            beginFound = 1;

        tokenCount++;
        int hashIndex, listIndex, lineNumber; // Variable to store hash index,list index,line number

        char lineNumberString[7], tokenTypeString[7]; // Array to store formatted line number and formatted token type

        lineNumberString[6] = '\0'; // Null terminate the array

        tokenTypeString[6] = '\0'; // Null terminate the array

        // Get current line number
        lineNumber = yylineno;

        if (token == 3)
        {
            if (isFull(&str))
            {
                printf("Elements can't be inserted as the stack is full\n");
                exit(1);
            }
            else
            {
                push(&str, ++beginNo);
            }
        }

        if (token == 4 || token == 5)
        {
            if (isEmpty(&str))
            {
                printf("Error: No BEGIN statement found for END no. %d\n", ++endNo);
                exit(1);
            }
            else
            {
                int prt = pop(&str);
                endNo++;
            }
        }

        // Format line number for output
        if (lineNumber != lastLine)
        {
            int cur = 5;           // Current index for formatting
            int temp = lineNumber; // Temporary variable to store line number

            // Convert line number to string
            for (; temp; temp /= 10)
            {
                lineNumberString[cur] = '0' + (temp % 10);
                cur--;
            }

            // Fill remaining spaces with blanks
            for (; cur >= 0; cur--)
                lineNumberString[cur] = ' ';
        }
        else
        {
            int cur = 5; // Current index for formatting

            // Fill entire string with blanks
            for (; cur >= 0; cur--)
                lineNumberString[cur] = ' ';
        }

        int cur = 5;      // Current index for formatting
        int temp = token; // Temporary variable to store token

        // Write line number to output file
        fprintf(fout, "%s  ", lineNumberString);

        lastLine = yylineno; // Update last line number

        // Format token type for output
        for (; temp; temp /= 10)
            tokenTypeString[cur--] = '0' + (temp % 10);

        // Fill remaining spaces with blanks
        for (; cur >= 0; cur--)
            tokenTypeString[cur] = ' ';

        // Write token type to output file
        fprintf(fout, "%s    ", tokenTypeString);

        // Handle identifier tokens
        if (token == 22)
        {
            char *variableName = yytext;                          // Get variable name
            install_id(variableName, 22, &listIndex, &hashIndex); // Install identifier into symbol table

            fprintf(fout, "^%s (hashIndex: %d, listIndex: %d)\n", yytext, hashIndex, listIndex); // Write identifier info to output file
        }
        // Handle number tokens
        else if (token == 23)
        {
            char *num = yytext;                           // Get number
            install_num(num, 23, &listIndex, &hashIndex); // Install number into symbol table

            fprintf(fout, "#%s (hashIndex: %d, listIndex: %d)\n", yytext, hashIndex, listIndex); // Write number info to output file
        }
        else
            fprintf(fout, "\n"); // Write newline to output file

        token = yylex(); // Get next token
    }

    int rtr = 1;
    int ch = 0;

    if (tokenCount == 0)
    {
        printf("Error : Input file is empty\n");
        exit(1);
    }

    if (programEnd == 0)
    {
        printf("Error : End of the program is not specified\n");
        exit(1);
    }
    while (beginNo > endNo)
    {
        printf("Error: Missing END statement for BEGIN statement %d\n", rtr++);
        beginNo--;
        ch = 1;
    }
    if (ch)
        exit(1);

    // Write hash table contents to output file

    fprintf(fout, "\n      HASH TABLE\n       ----------\n(non-empty entries only)\n\n   INDEX   TOKEN TYPE   TOKEN SPECIFIER\n   -----   ----------   ---------------\n");

    int lastIndex = -1;                          // Initialize last index
    char hashIndexString[7], tokenTypeString[7]; // Array to store formatted hash index and formatted token type

    hashIndexString[6] = '\0'; // Null terminate the array
    tokenTypeString[6] = '\0'; // Null terminate the array

    // Loop through hash table slots
    for (int i = 0; i < 400; i++)
    {
        // Check if the slot is non-empty
        if (numberOfElements[i] > 0)
        {
            struct node *itr = hashTable[i]; // Get pointer to first node in linked list

            // Iterate through the linked list
            while (itr != NULL)
            {
                // Check if the current index is different from the last index
                if (lastIndex != i)
                {
                    // Format hash index for output
                    if (i == 0)
                        strcpy(hashIndexString, "     0"); // Special case for index 0
                    else
                    {
                        int cur = 5;  // Current index for formatting
                        int temp = i; // Temporary variable to store index

                        // Convert index to string
                        for (; temp; temp /= 10)
                            hashIndexString[cur--] = '0' + (temp % 10);

                        // Fill remaining spaces with blanks
                        for (; cur >= 0;)
                            hashIndexString[cur--] = ' ';
                    }
                }
                else
                {
                    int cur = 5; // Current index for formatting

                    // Fill entire string with blanks
                    for (; cur >= 0;)
                        hashIndexString[cur--] = ' ';
                }

                lastIndex = i; // Update last index

                // Write hash index to output file
                fprintf(fout, "%s    ", hashIndexString);

                int temp = itr->value; // Temporary variable to store value
                int cur = 5;           // Current index for formatting

                // Format token type for output
                for (; temp; temp /= 10)
                    tokenTypeString[cur--] = '0' + (temp % 10);

                // Fill remaining spaces with blanks
                for (; cur >= 0;)
                    tokenTypeString[cur--] = ' ';

                // Write token type to output file
                fprintf(fout, "%s          ", tokenTypeString);
                // Check if the value corresponds to an identifier or number

                if (itr->value == 22)
                    fprintf(fout, "^%s\n", itr->key); // Write identifier to output file
                else if (itr->value == 23)
                    fprintf(fout, "#%s\n", itr->key); // Write number to output file

                // Move to the next node in the linked list
                itr = itr->next;
            }
        }
    }
    fclose(fout); // Close output file

    return 0; // Return 0 to indicate successful execution
}
