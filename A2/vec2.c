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

#include "a2.h"

/* 
* Calculates the cross product of 2D vectors. 
* 
* @discussion	Used to find out if Vector2 c
*				is to the left, right, or colinear
*				with Vector2 a.
* 
*/
int cprod(Vector2* a, Vector2* b, Vector2* c)
{
	float x1, x2, y1, y2, prod;

	x1 = a->x - b->x;
	x2 = a->x - c->x;
	y1 = a->y - b->y;
	y2 = a->y - c->y;

	prod = (y2 * x1) - (y1 * x2);

	if (prod < 0) return -1;	// right
	if (prod > 0) return 1;		// left
	return 0;	// colinear
}

/*
* Determine if a vector exists within an array.
* 
* @param arr - an array of vectors
* @param search - the vector to search for
* @param sz - the size of the array
* 
* @returns 1 if found, 0 otherwise
*/
int contains(Vector2** arr, Vector2* search, int sz) 
{
	for (register int i = 0; i < sz; ++i)
	{
		if (vec2equ(arr[i], search))
			return 1;
	}

	return 0;
}

/* Calculates the distance between two 2D vectors. */
float dist(Vector2* a, Vector2* b)
{
	float xSqr, ySqr;

	xSqr = a->x - b->x;
	xSqr *= xSqr;

	ySqr = a->y - b->y;
	ySqr *= ySqr;

	return sqrt(xSqr + ySqr);
}

/* 
* Calculates the distance between a vector and a line. 
* 
* @param a - first vector on line
* @param b - second vector on line
* @param c - vector to find distance from line
* 
* @returns absolute distance
*/
float linedist(Vector2* a, Vector2* b, Vector2* c)
{
	float w, x, y, z;

	w = c->y - a->y;
	x = b->x - a->x;
	y = b->y - a->y;
	z = c->x - a->x;

	return abs((x * w) - (y * z));
}

/*
* Checks if two 2D vectors are equal.
* 
* @returns 1 if equal, 0 otherwise.
*/
int vec2equ(Vector2* a, Vector2* b)
{
	float x1, x2, y1, y2;

	/* 
	* Round all values within 1 decimal place 
	* before comparing.
	*/
	x1 = roundf(a->x * 0.1f) * 10;
	x2 = roundf(b->x * 0.1f) * 10;

	y1 = roundf(a->y * 0.1f) * 10;
	y2 = roundf(b->y * 0.1f) * 10;

	if (x1 == x2 && y1 == y2)
		return 1;

	return 0;
}

/*
* Search for the 2D vector with the lowest x value.
* 
* @param arr - array to search
* @param sz - size of the array
* 
* @returns 2D vector with lowest x value.
*/
Vector2* minx(Vector2** arr, int sz)
{
	if (sz == 0)
		return NULL;

	Vector2* min = arr[0];

	/*
	* Iterate through the entire array,
	* finding the vector with the lowest 
	* x value.
	*/ 
	for (register int i = 1; i < sz; i++)
		if (arr[i]->x < min->x)
			min = arr[i];

	return min;
}
