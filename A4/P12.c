/*
* Assignment: 4
* Date: 03/28/2022
*
* Name: Justin Erogun
* ID: 1144036
*/

#include "a4.h"

/* Recursively search the BST. */
bool search_greedy(Node*, char*);

/* Recursively create the BST. */
Node* make_greedy(List*, Node*, int, int);

void BST_greedy(List* list)
/*
* The main Greedy BST algorithm. Responsible for 
* invoking the BST creation and BST searching methods.
*/
{
	Node* root = NULL;
	char input[BUFSIZ];
	
	root = make_greedy(list, root, 0, list->size - 1);

	printf("Enter a key: ");
	scanf("%s", input);

	search_greedy(root, input);

	// Free allocated memory.
	freeNodes(root);
}

bool search_greedy(Node* root, char* str)
/*
* The Greedy BST searching function. Similar to that of
* search_optimal aside from the float used in the output.
* 
* Recursively searches for a word within the BST using strcmp
* while outputting the results of the search.
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

	printf("Compared with %s (%.3f), ", root->data->word, root->data->freq);

	if (ret == 0)
	{
		printf("found.\n");
		return true;
	}
	else if (ret > 0)
	{
		printf("go left subtree.\n");
		return search_greedy(root->left, str);
	}
	else if (ret < 0)
	{
		printf("go right subtree.\n");
		return search_greedy(root->right, str);
	}
}

Node* make_greedy(List* words, Node* root, int l, int r)
/*
* The Greedy BST creation function. Creates the tree by
* using frequency as the deciding factor for Node location.
*/
{
	// Base case
	if (l > r)
		return NULL;

	float m_freq = -1.0f;
	int index = 0;

	// Iterate over subarray to find the Word with the highest frequency.
	for (int i = l; i <= r; ++i)
	{
		Word* w = words->data[i];

		if (w && w->freq > m_freq)
		{
			m_freq = w->freq;
			index = i;
		}
	}
	
	root = malloc(sizeof(Node));

	if (!root)
		return NULL;

	// Initialize the node.
	root->data = words->data[index];
	root->cost = 0;
	root->left = NULL;
	root->right = NULL;

	// Make the left subtree.
	root->left = make_greedy(words, root->left, l, index - 1);

	// Make the right subtree.
	root->right = make_greedy(words, root->right, index + 1, r);

	return root;
}
