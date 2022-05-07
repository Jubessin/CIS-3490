/*
* Assignment: 4
* Date: 03/28/2022
*
* Name: Justin Erogun
* ID: 1144036
*/

#include "a4.h"

/* Read the file that will be used to construct the BSTs. */
char* readBSTFile();

/* Get the list of unique words. */
List* getWords(char*);

/* Read the file that will be used to construct the matrix. */
int* readMatFile(int*);

/* Free the list pointer and all memory. */
void freeList(List* list);

/* 
* Check if a word exists within the list of Word structures. 
* 
* @returns a pointer to a Word that was found, or NULL
*/
Word* exists(List* list, char* cmp);

/* Compares two strings found in Word structures using strcmp. */
int compareWords(const void*, const void*);

int main(int argc, char* argv[])
{
	int n = 0;
	int* mat = NULL;
	char* ftext = NULL;

	ftext = readBSTFile();
	mat = readMatFile(&n);

	if (!ftext || !mat)
	{
		if (ftext)
			free(ftext);

		if (mat)
			free(mat);

		fprintf(stderr, "Error reading in files.\n");

		return 1;
	}

	// Get the list of unique words in the text of the read file.
	List* list = getWords(ftext);

	// First index of the list is NULL, skip it.
	void** listptr = list->data;
	++listptr;

	// Alphabetically sort the list of words using built-in quick sort.
	qsort(listptr, list->size - 1, sizeof(Word*), compareWords);

	printf("\n--P1.1 Optimal BST --\n\n");
	BST_optimal(list);

	printf("\n\n--P1.2 Greedy BST --\n\n");
	BST_greedy(list);

	printf("\n\n--P2 Stable Marriage --\n\n");
	stable_marriage(mat, n);

	// Free allocated memory.
	freeList(list);
	free(ftext);
	free(mat);

	return 0;
}

char* readBSTFile()
/*
* Read a file to construct the BST, and return the text
* (consisting of alphabet and space characters) found in the file.
*/
{
	int* mat = NULL;

	FILE* fp = NULL;

	if ((fp = fopen(P1FILE, "r")) == NULL)
	{
		fprintf(stderr, "Error opening %s\n", P1FILE);
		return NULL;
	}

	char c;
	long fsize;
	int index = 0;
	char *ftext = NULL;

	// Move to end of file.
	fseek(fp, 0, SEEK_END);

	// Get length of file.
	fsize = ftell(fp);

	// Allocate memory for characters in file.
	ftext = malloc(sizeof(char) * (fsize + 1));

	// Move to start of file.
	fseek(fp, 0, SEEK_SET);

	// Read every space, and alphabet character.
	while ((c = fgetc(fp)) != EOF)
		if (isalpha(c) || isspace(c))
			ftext[index++] = c;

	fclose(fp);

	// Null-terminate.
	ftext[index] = '\0';

	return ftext;
}

int* readMatFile(int* n)
/*
* Read a file to construct an array containing two n*n sized matrices.
*/
{
	char file[BUFSIZ];
	int* mat = NULL;

	FILE* fp = NULL;

	printf("Please enter a filename for P2: ");
	scanf("%s", file);

	if ((fp = fopen(file, "r")) == NULL)
	{
		fprintf(stderr, "Error opening %s\n", file);
		return NULL;
	}

	char* buf = NULL;
	size_t bufsz = sizeof(char) * BUFSIZ;
	int index = 0;

	while ((getline(&buf, &bufsz, fp) != EOF))
	{
		int i1, i2, i3, i4, ret;

		ret = sscanf(buf, "%d %d %d %d", &i1, &i2, &i3, &i4);

		// First line of file, indicating size of matrix.
		if (ret == 1)
		{
			*n = i1;

			// Allocate space for two matrices.
			mat = malloc(sizeof(int) * (i1 * i1 * 2));
		}
		// Row of a matrix
		else if (ret >= 3)
		{
			mat[index++] = i1;
			mat[index++] = i2;
			mat[index++] = i3;
			
			if (ret == 4)
			{
				mat[index++] = i4;
			}
		}
	}

	if (buf)
		free(buf);

	fclose(fp);

	return mat;
}

void freeList(List* list)
/*
* Free the allocation of memory used in a List structure 
* composed of Word structures.
*/
{
	if (!list)
		return;

	if (list->data)
	{
		for (int i = 0; i < list->size; ++i)
		{
			Word* w = list->data[i];

			if (w)
			{
				free(w->word);
				free(w);
			}
		}

		free(list->data);
	}

	free(list);
}

void freeNodes(Node* root)
/*
* Recursively free all attached nodes.
*/
{
	// Base case.
	if (!root)
		return;

	// Free left subtree.
	if (root->left)
		freeNodes(root->left);

	// Free right subtree.
	if (root->right)
		freeNodes(root->right);

	free(root);
}

List* getWords(char* text)
/*
* Get all unique words found in text, converting them into Word structures
* before appending them to the List and returning the List structure.
*/
{
	List* list = malloc(sizeof(List));

	if (!list)
		return NULL;

	// Initialize the list.
	list->data = NULL;
	list->size = 1;

	char
		* dup = NULL,				// duplicate of the text
		* token = NULL;				// used in tokenizing

	dup = strdup(text);
	token = strtok(dup, " \n\r\t");

	Word* tmp = NULL;

	// Tokenize all words in the text (using duplicate).
	while (token)
	{
		tmp = exists(list, token);

		// If the word is exists in the list
		if (tmp)
		{
			tmp->count++;
			tmp->freq = (float)tmp->count / NUM_WORDS;
		}
		else
		{
			// Allocate space for the unique word.
			tmp = malloc(sizeof(Word));

			if (!tmp)
				continue;

			tmp->word = malloc(sizeof(char) * (strlen(token) + 1));

			if (!tmp->word)
			{
				free(tmp);
				continue;
			}

			strcpy(tmp->word, token);

			tmp->count = 1;
			tmp->freq = (float)tmp->count / NUM_WORDS;

			// Reallocate the list to fit the new node.
			list->data = realloc(list->data, sizeof(Word*) * (list->size + 1));
			list->data[list->size++] = tmp;
		}

		token = strtok(NULL, " \n\r\t");
	}

	// Assign the first index of the list to NULL, to match the tables used in P1.1.
	list->data[0] = NULL;

	if (dup)
		free(dup);

	return list;
}

Word* exists(List* list, char* cmp)
/*
* Check if the cmp string exists in the List of 
* Word structures.
*/
{
	if (!list || !list->data)
		return NULL;

	// Skip the first NULL index.
	for (int i = 1; i < list->size; ++i)
	{
		Word* w = list->data[i];

		if (!w)
			continue;

		if (strcmp(w->word, cmp) == 0)
			return w;
	}

	return NULL;
}

int compareWords(const void* word1, const void* word2)
{
	// Necessary casting conversion in order to get past the const keyword.
	const Word* w1 = *(const Word**)word1;
	const Word* w2 = *(const Word**)word2;

	return strcmp(w1->word, w2->word);
}
