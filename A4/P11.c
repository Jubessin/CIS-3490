/*
* Assignment: 4
* Date: 03/28/2022
*
* Name: Justin Erogun
* ID: 1144036
*/

#include "a4.h"

/* Recursively search BST. */
bool search_optimal(Node*, char*);

/* Make the root and main tables. */
void make_tables(List*, float**, int**);

/* Recursively make the BST */
Node* make_optimal(float**, int**, List*, Node*, int, int);

void BST_optimal(List* list)
/*
* The main Optimal BST function. Responsible for invoking the
* table creation, tree creation, and tree searching methods.
*/
{
	// + 2, to account for the main and root table sizes.
	int size = list->size + 2;

	float** m_table = NULL;
	int** r_table = NULL;

	char input[BUFSIZ];

	Node* root = NULL;

	// Allocate the tables.
	m_table = malloc(sizeof(float*) * size);

	if (!m_table)
	{
		return;
	}

	r_table = malloc(sizeof(int*) * size);

	if (!r_table)
	{
		free(m_table);
		return;
	}

	make_tables(list, m_table, r_table);

	// Make the optimal BST. Start at index 1 to avoid the NULL at 0.
	root = make_optimal(m_table, r_table, list, root, 1, list->size - 1);

	printf("Enter a key: ");
	scanf("%s", input);

	search_optimal(root, input);

	// Free allocated memory.
	for (int i = 0; i < size; ++i)
	{
		free(m_table[i]);
		free(r_table[i]);
	}

	free(m_table);
	free(r_table);

	freeNodes(root);
}

bool search_optimal(Node* root, char* str)
/*
* Recursively search the BST while outputting 
* traversal and cost of nodes.
*/
{
	// Base case.
	if (!root || !root->data || !root->data->word || !str)
	{
		printf("Not found.\n");
		return false;
	}

	// Compare strings for equality.
	int ret = strcmp(root->data->word, str);

	printf("Compared with %s (%.3f), ", root->data->word, root->cost);

	if (ret == 0)
	{
		printf("found.\n");
		return true;
	}
	else if (ret > 0)
	{
		printf("go left subtree.\n");
		return search_optimal(root->left, str);
	}
	else if (ret < 0)
	{
		printf("go right subtree.\n");
		return search_optimal(root->right, str);
	}
}

void make_tables(List* words, float** m_table, int** r_table)
/* 
* Used the algorithm found in the textbook, with some slight modifications. 
*/
{

	int 
		j, 
		n, 
		kmin = 0;

	float 
		minval, 
		sum = 0;

	n = words->size;

	// Allocate (and initialize) tables to 0.
	for (int i = 0; i < (n + 2); ++i)
	{
		m_table[i] = calloc(n + 2, sizeof(float));
		r_table[i] = calloc(n + 2, sizeof(int));
	}

	for (int i = 1; i < n; ++i)
	{
		Word* w = words->data[i];

		m_table[i][i - 1] = 0.0f;
		m_table[i][i] = w->freq;
		r_table[i][i] = i;
	}

	m_table[n + 1][n] = 0;

	for (int d = 1; d < (n - 1); ++d)
	{
		for (int i = 1; i < (n - d); ++i)
		{
			j = i + d;
			minval = INFINITY;

			for (int k = i; k < j; ++k)
			{
				float val = (m_table[i][k - 1] + m_table[k + 1][j]);

				if (val < minval)
				{
					minval = val;
					kmin = k;
				}
			}

			r_table[i][j] = kmin;

			Word* word_i = words->data[i];

			if (word_i)
			{
				sum = word_i->freq;

				// Start at i instead of i + 1. 
				// Issues with getting correct cost values otherwise.
				for (int s = i; s < j; ++s)
				{
					Word* word_s = words->data[s];

					if (word_s)
						sum += word_s->freq;
				}
			}

			m_table[i][j] = minval + sum;
		}
	}
}

Node* make_optimal(float** m_table, int** r_table, List* words, Node* root, int l, int r)
/*
* Create the Optimal BST using the main and root table to find 
* the indices of nodes.
*/
{
	// Base case.
	if (l > r)
		return NULL;

	// Get the Word index from the root table.
	int index = r_table[l][r];

	root = malloc(sizeof(Node));

	if (!root)
		return NULL;

	// Initialize the node.
	root->cost = m_table[l][r];
	root->data = words->data[index];
	root->left = NULL;
	root->right = NULL;

	// Create left sub tree.
	root->left = make_optimal(m_table, r_table, words, root->left, l, index - 1);

	// Create right sub tree.
	root->right = make_optimal(m_table, r_table, words, root->right, index + 1, r);

	return root;
}
