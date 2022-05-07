/*
* Assignment: 4
* Date: 03/28/2022
* 
* Name: Justin Erogun
* ID: 1144036
*/

#ifndef A4_H
#define A4_H

// Required to address getline warning
#define _GNU_SOURCE		

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>

#define NUM_WORDS 2045
#define P1FILE "data_A4_Q1.txt"

typedef struct Word_t
{
	int count;				// number of times word is in text
	float freq;				// frequency of the word in the text
	char* word;
}Word;

typedef struct Node_t
{
	float cost;

	struct Node_t* left;
	struct Node_t* right;

	Word* data;
}Node;

typedef struct List_t
{
	int size;
	void** data;
}List;

/* Run the P1.1 (Optimal BST) algorithm. */
void BST_optimal(List*);

/* Run the P1.2 (Greedy BST) algorithm. */
void BST_greedy(List*);

/* Run the P2 (Stable Marriage) algorithm. */
void stable_marriage(int* mat, int n);

/* Recursively free all attached nodes. */
void freeNodes(Node*);

#endif
