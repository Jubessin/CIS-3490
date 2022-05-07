/*
*
* Course: CIS*3490 The Design and Analysis of Algorithms
* Assignment: 2
*
* Name: Justin Erogun
* ID: 1144036
* Date: 02/12/2022
*
*/

#ifndef A2_H
#define A2_H

#define _GNU_SOURCE		// Required to address getline warning

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

typedef struct Vector2_t	// Point structure
{
	float x;	// x component of the vector.
	float y;	// y component of the vector.
}Vector2;

typedef struct Hull_t	// Hull structure
{
	Vector2** points;
	int sz;
} Hull;

#define P1_FILE "data_A2_Q1.txt"	// name of p1 file
#define P1_SIZE 50000				// number of integers

#define P2_FILE "data_A2_Q2.txt"	// name of p2 file
#define P2_SIZE 30000				// number of x-y points

// Brute Force Algorithm for counting inversions.
int bf_inversion(int*);

// Divide and Conquer Algorithm (Merge Sort) for counting inversions.
int dc_inversion(int*);

// Brute Force Algorithm for getting extreme points of a convex hull.
Hull* bf_hull(Vector2**);

// Divide and Conquer Algorithm (Quickhull) for getting extreme points of a convex hull.
Hull* dc_hull(Vector2**);

int cprod(Vector2*, Vector2*, Vector2*);
int contains(Vector2**, Vector2*, int);
int vec2equ(Vector2*, Vector2*);
float linedist(Vector2*, Vector2*, Vector2*);
float dist(Vector2*, Vector2*);
Vector2* minx(Vector2**, int);

#endif
