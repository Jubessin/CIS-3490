/*
* Assignment: 4
* Date: 03/28/2022
*
* Name: Justin Erogun
* ID: 1144036
*/

#include "a4.h"

typedef struct Person_t
{
	int p_id;				// id of the partner
	int p_index;			// index of the partner (indicates their preference, lower is better)
	int preferences[];		// array of preferences (ids at indexes)
}Person;

/* Get the ID of the most preferred woman that has not rejected the man. */
int bestID(Person*, int);

/* Check if all people have been paired. */
bool allPaired(Person**, int);

/* Attempts to partner a man with his most preferred woman. */
void propose(Person**, Person**, int, int, int);

void stable_marriage(int* mat, int n)
{
	int i = 0;
	int nsqr = n * n;

	// Allocate arrays.
	Person** men = malloc(sizeof(Person*) * n);

	if (!men)
	{
		return;
	}

	Person** women = malloc(sizeof(Person*) * n);

	if (!women)
	{
		free(men);
		return;
	}

	// Populate male array.
	for (i = 0; i < n; ++i)
	{
		Person* p = malloc(sizeof(Person) + (sizeof(int) * n));

		if (!p)
		{
			men[i] = NULL;
			continue;
		}

		// Initialize struct.
		p->p_index = -1;
		p->p_id = -1;

		for (int j = 0; j < n; ++j)
			p->preferences[j] = mat[(i * n) + j];

		men[i] = p;
	}

	// Populate female array.
	for (i = 0; i < n; ++i)
	{
		Person* p = malloc(sizeof(Person) + (sizeof(int) * n));

		if (!p)
		{
			women[i] = NULL;
			continue;
		}

		// Initialize struct.
		p->p_index = -1;
		p->p_id = -1;

		for (int j = 0; j < n; ++j)
			p->preferences[j] = mat[(i * n) + nsqr + j];

		women[i] = p;
	}

	// Reset index.
	i = 0;

	// Iterate until every person is paired. 
	// Should finish within less than n^2 iterations.
	while (true)
	{
		// Reset if anyone is not paired.
		if (i >= n)
		{
			if (!allPaired(men, n))
				i = 0;
			else 
				break;
		}

		// Check if the current male is not paired.
		if (men[i]->p_index == -1)
		{
			// Get the ID of the best partner.
			int best = bestID(men[i], n);

			// Shouldn't happen, but check for -1 just in case.
			if (best != -1)
			{
				// IDs are not zero-based, so add 1 to i.
				propose(men, women, i + 1, best, n);
			}
		}

		++i;
	}

	// Print results, and free allocated memory.
	for (i = 0; i < n; ++i)
	{
		if (men[i])
		{
			for (int j = 1; j < n + 1; ++j)
			{
				if (j == men[i]->p_id)
					printf("1 ");
				else
					printf("0 ");
			}
			printf("\n");

			free(men[i]);
		}
		if (women[i])
		{
			free(women[i]);
		}
	}

	free(men);
	free(women);
}

int bestID(Person* man, int n)
{
	for (int i = 0; i < n; ++i)
		if (man->preferences[i] != -1)
			return man->preferences[i];

	return -1;
}

bool allPaired(Person** men, int n)
{
	// Iterate over all men, checking if they're paired.
	for (int i = 0; i < n; ++i)
		if (men[i]->p_index == -1)
			return false;

	return true;
}

void propose(Person** men, Person** women, int m_id, int f_id, int n)
{
	// IDs are not zero-based, subtract IDs by 1.
	Person* man = men[m_id - 1];
	Person* woman = women[f_id - 1];

	// If the woman is not paired, she accepts.
	if (woman->p_index == -1)
	{
		for (int i = 0; i < n; ++i)
		{
			// Compare ID to ID.
			if (woman->preferences[i] == m_id)
			{
				// Assign the woman's index and id as the man's.
				woman->p_index = i;
				woman->p_id = woman->preferences[i];

				break;
			}
		}

		// Assign the man's index and id as the woman's.
		man->p_id = f_id;

		for (int i = 0; i < n; ++i)
		{
			if (man->preferences[i] == f_id)
			{
				man->p_index = i;
				break;
			}
		}
	}
	else
	{
		int tmp_id = -1;			// proposing man's id
		int tmp_index = -1;			// proposing man's index

		for (int i = 0; i < n; ++i)
		{
			// Compare ids.
			if (woman->preferences[i] == m_id)
			{
				tmp_index = i;
				tmp_id = woman->preferences[i];

				break;
			}
		}

		// Proposing man is more preferable to current partner.
		if (tmp_index < woman->p_index)
		{
			// Set the man's preference id at the woman's index to -1 (to stop further proposals).
			men[woman->p_id - 1]->preferences[men[woman->p_id - 1]->p_index] = -1;

			// Set partner id to default, as man is no longer partnered.
			men[woman->p_id - 1]->p_id = -1;

			// Set partner index to default, as man is no longer partnered.
			men[woman->p_id - 1]->p_index = -1;

			// Set woman's id and index as that of the proposing man.
			woman->p_id = tmp_id;
			woman->p_index = tmp_index;

			// Set proposing man's id and index as that of the woman.
			man->p_id = f_id;

			for (int i = 0; i < n; ++i)
			{
				if (man->preferences[i] == f_id)
				{
					man->p_index = i;
					break;
				}
			}
		}
		// Current partner is more preferable to proposing partner.
		else
		{
			for (int i = 0; i < n; ++i)
			{
				if (man->preferences[i] == f_id)
				{
					// Set the proposing man's preference id at the woman's index to -1 (to stop further proposals).
					man->preferences[i] = -1;
					break;
				}
			}
		}
	}
}