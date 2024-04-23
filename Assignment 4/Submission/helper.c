// help.c
#include <stdio.h>	// Including standard input-output library
#include <string.h> // Including string manipulation functions
#include <stdlib.h> // Including standard library functions
#include <stdint.h> // Including integer types

extern int yylineno; // Declaration of an external variable yylineno

void yyerror(const char *s); // Declaration of a function to handle parsing errors

// Structure to define a node in a linked list
struct node
{
	char key[200];	   // String key
	int type;		   // Type of the node
	struct node *next; // Pointer to the next node
};

// Variables to track previous expression type, mismatch type, line number, and error index
int previousExpressionType = -1;
int mismatchType = 0;
int lineNo = -1;
int errorIndex = 1;

// to check if there is a syntax error
int syntaxError = 0;

// Array of pointers to nodes (symbol table)
struct node *hashTable[400];
struct node *lastNode[400];
int num_elements[400];

// Function to reset error-related variables
void resetAllVariables(int code)
{
	previousExpressionType = -1;
	mismatchType = 0;
	if (code == 1)
		lineNo = -1;
}

// Function to concatenateTwoStrings two strings
void concatenateTwoStrings(char a[], char b[], char result[])
{
	int i = 0;
	int j = 0;
	while (a[j])
		result[i] = a[j], i++, j++;
	result[i] = ','; // Adding a comma as a separator
	i++, j = 0;
	while (b[j])
		result[i] = b[j], i++, j++;
	result[i] = '\0'; // Null-terminating the resulting string
}

// Function to create a new node in the linked list
struct node *createNode(char *key, int type)
{
	struct node *temp = (struct node *)malloc(sizeof(struct node)); // Allocating memory for the new node

	strcpy(temp->key, key); // Copying the key
	temp->type = type;		// Assigning the type
	temp->next = NULL;		// Setting the next pointer to NULL

	return temp;
}

// Murmur Hashing algorithm
uint64_t hashFunction(char *key)
{
	uint64_t h = 525201411107845655ull;
	for (; *key; ++key)
	{
		h ^= *key;
		h *= 0x5bd1e9955bd1e995;
		h ^= h >> 47;
	}
	return h;
}

// Function to add a new identifier to the symbol table
void addNewIdentifier(char key[], int type)
{
	uint64_t hash = hashFunction(key); // Hashing the key to get an index

	int hashIdx = hash % 400; // Modulo operation to get the index within the array size

	struct node *itr = hashTable[hashIdx]; // Getting the linked list at the index

	// Checking if the identifier already exists in the symbol table
	while (itr != NULL)
	{
		if (strcmp(itr->key, key) == 0)
		{
			// If it exists, output an error message and return
			FILE *outputFile;
			outputFile = fopen("errors.txt", "a");
			if (!syntaxError)
				fprintf(outputFile, "ERROR %d type SEMANTIC: Identifier %s already exists in symbol table on line number %d\n", errorIndex, key, yylineno);
			errorIndex++;
			fclose(outputFile);
			return;
		}

		itr = itr->next; // Move to the next node
	}

	// If the identifier doesn't exist, add it to the symbol table
	struct node *newNode = createNode(key, type); // Creating a new node

	if (num_elements[hashIdx] == 0)
	{
		hashTable[hashIdx] = newNode; // If it's the first element in the linked list
		lastNode[hashIdx] = newNode;  // Marking it as the last node as well
		num_elements[hashIdx]++;	  // Incrementing the number of elements
	}
	else
	{
		lastNode[hashIdx]->next = newNode; // Adding the new node to the end of the list
		lastNode[hashIdx] = newNode;	   // Updating the last node pointer
		num_elements[hashIdx]++;		   // Incrementing the number of elements
	}
}

// Function to extract a substring from a string
void extractSubstring(char s[], char key[], int left, int right)
{

	int i = left;
	// Copying characters from 'left' to 'right' indices into 'key'
	while (i <= right)
		key[i - left] = s[i], i++;
	int endd = right + 1 - left;
	key[endd] = '\0'; // Null-terminating the key
}

// Function to add a comma-separated list of identifiers to the symbol table
void addListOfIdentifiers(char s[], int type)
{
	char delimiter = ','; // Delimiter for splitting the list
	char key[200];
	int left = -1, right = 0;

	while (s[right])
	{
		if (s[right] == delimiter)
		{
			extractSubstring(s, key, left + 1, right - 1); // Extracting each identifier
			addNewIdentifier(key, type);				   // Adding it to the symbol table
			left = right;								   // Updating the left index
		}
		right++;
	}
	// Taking care of the last identifier
	extractSubstring(s, key, left + 1, right - 1);
	addNewIdentifier(key, type);
}

// Function to get the type of an identifier from the symbol table
void getIdentifierType(char key[], int *type)
{
	uint64_t hash = hashFunction(key); // Hashing the key

	int hashIdx = hash % 400; // Getting the index within the array size

	struct node *itr = hashTable[hashIdx]; // Getting the linked list at the index

	while (itr != NULL)
	{
		// If the identifier is found
		if (strcmp(itr->key, key) == 0)
		{
			*type = itr->type; // Assigning its type
			return;
		}

		itr = itr->next; // Move to the next node
	}
	// If the identifier is not found in the symbol table
	*type = -200;

	FILE *outputFile;
	outputFile = fopen("errors.txt", "a");
	if (!syntaxError)
		fprintf(outputFile, "ERROR %d type SEMANTIC: Identifier %s does not exist on line number %d\n", errorIndex, key, lineNo);
	errorIndex++;
	fclose(outputFile);
}

// Function to verify whether a comma-separated list of identifiers is present in the symbol table
void verifyListOfIdentifiers(char s[])
{
	char delimiter = ','; // Delimiter for splitting the list
	char key[200];
	int left = -1, right = 0;

	while (s[right])
	{
		if (s[right] == delimiter)
		{
			extractSubstring(s, key, left + 1, right - 1); // Extracting each identifier
			int type;
			getIdentifierType(key, &type); // Getting its type
			left = right;				   // Updating the left index
		}
		right++;
	}
	// Taking care of the last identifier
	extractSubstring(s, key, left + 1, right - 1);
	int type;
	getIdentifierType(key, &type);
}

void print_hash_table()
{
	// Pretty printing of hash table
	FILE *hashTableFile;

	// Open the file in append mode
	hashTableFile = fopen("hashTable.txt", "a");
	fprintf(hashTableFile,"-------------------------- HASH TABLE ------------------------------\n");
	for (int i = 0; i < 400; i++)
	{
		struct node *temp = hashTable[i];
		if (num_elements[i] > 0)
		{
			while (temp != NULL)
			{
				fprintf(hashTableFile,"index %d => ", i);
				fprintf(hashTableFile,"<%s,%d>", temp->key, temp->type);
				fprintf(hashTableFile," -> ");
				temp = temp->next;
			}
			fprintf(hashTableFile,"NULL\n");
		}
	}

	fclose(hashTableFile);


	FILE *errorsFile = fopen("errors.txt", "r");
    if (errorsFile != NULL) {
        fclose(errorsFile);
        // Delete hashTable.txt
        if (remove("hashTable.txt") == 0) {
            // printf("hashTable.txt deleted successfully.\n");
        } else {
            // printf("Error deleting hashTable.txt.\n");
        }
    } else {
        // printf("errors.txt does not exist.\n");
    }
}

// Function to handle parsing errors
void yyerror(const char *s)
{
	FILE *outputFile;
	outputFile = fopen("errors.txt", "a");
	if (strcmp(s, "syntax error") == 0)
		syntaxError = 1;
	fprintf(outputFile, "ERROR %d type SYNTACTIC: %s ON LINE NUMBER %d\n", errorIndex, s, yylineno);
	fclose(outputFile);
	errorIndex++;
}
