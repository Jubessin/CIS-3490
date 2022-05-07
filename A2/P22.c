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

void _dc_hull(Hull*, Vector2**, Vector2*, Vector2*, int);

Hull* dc_hull(Vector2** data)
/*
* Global function, called from main.
* 
* This first finds both the vectors with the 
* minimum and maximum x values within the given data.
* 
* It then intersects the data by placing all vectors
* into either of two arrays, dependent on whether
* each vector is to the left or the right of the
* line created by the minimum and maximum x value
* vectors.
* 
* Lastly, the "private" or "internal" function
* _dc_hull is called using the array that were populated
* by this intersection of vectors.
*/
{
	Hull* hull = malloc(sizeof(Hull));

	Vector2 
		**left = NULL,		// array of vectors left of the intersection line
		**right = NULL;		// array of vectors, right of the intersection line

	Vector2 
		*max = NULL,		// vector with the maximum x value
		*min = NULL;		// vector with the minimum x value

	int 
		leftsz = 0,			// size of the left array
		rightsz = 0;		// size of the right array

	/* Initialize hull variables */
	hull->points = NULL;
	hull->sz = 0;

	// Start looking from the first vector in data.
	min = max = data[0];

	/* 
	* Iterate over all vectors in data. 
	* 
	* Find the minimum and maximum vectors with
	* respect to their x value.
	*/
	for (register int i = 1; i < P2_SIZE; i++)
	{
		if (min->x > data[i]->x)
			min = data[i];

		if (max->x < data[i]->x)
			max = data[i];
	}

	/* 
	* Iterate over all vectors in data. 
	* 
	* Assign vectors to either the left or
	* right array depending on their cross product
	* with the min and max vectors.
	*/
	for (register int i = 0; i < P2_SIZE; i++)
	{
		if (vec2equ(min, data[i]) || vec2equ(max, data[i]))
			continue;

		float cproduct = cprod(min, max, data[i]);

		if (cproduct == 1)
		{
			left = realloc(left, sizeof(Vector2*) * (leftsz + 1));

			left[leftsz++] = data[i];
		}
		else
		{
			right = realloc(right, sizeof(Vector2*) * (rightsz + 1));

			right[rightsz++] = data[i];
		}
	}

	// Recursively look for extremes in the left array.
	_dc_hull(hull, left, min, max, leftsz);

	// Recursively look for extremes in the right array.
	_dc_hull(hull, right, max, min, rightsz);

	if (left)
		free(left);

	if (right)
		free(right);

	return hull;
}

void _dc_hull(Hull* hull, Vector2** arr, Vector2* min, Vector2* max, int arrsz)
/*
* Local function, implementation of dc_hull function and quickhull algorithm.
* 
* This function first attempts to find the vector among the array 
* passed in that is the farthest from the line created
* by the min and max vectors.
* 
* If no such vector is found, then the min and max vectors
* are known to be points on the hull and are added before 
* returning.
* 
* If a vector is found, the function intersects the
* vectors in the array by two lines. One formed by the min 
* and farpoint vectors, and the other formed by the farpoint
* and max vectors.
* 
* The arrays created from this are then passed into
* separate recursive calls, until the base case
* is found.
*/
{
	// Farthest point from line formed by min and max.
	Vector2* farpoint = NULL;

	float 
		max_dist = 0,		// distance of farthest vector from line
		tmp_dist = 0;

	/* 
	* Iterate over all vectors in array. 
	* 
	* Find the farthest vector from min and max 
	* in the array.
	*/
	for (register int i = 0; i < arrsz; i++)
	{
		// Skip if vector is within the triangle 
		// formed by min, max, and arr[i].
		if (cprod(min, max, arr[i]) < 0)
			continue;

		tmp_dist = linedist(min, max, arr[i]);

		if (tmp_dist > max_dist)
		{
			max_dist = tmp_dist;
			farpoint = arr[i];
		}
	}

	// Base case
	if (!farpoint)
	{
		// If not present, add the min vector to the hull.
		if (contains(hull->points, min, hull->sz) == 0)
		{
			hull->points = realloc(hull->points, sizeof(Vector2*) * (hull->sz + 1));
			hull->points[hull->sz++] = min;
		}

		// If not present, add the max vector to the hull.
		if (contains(hull->points, max, hull->sz) == 0)
		{
			hull->points = realloc(hull->points, sizeof(Vector2*) * (hull->sz + 1));
			hull->points[hull->sz++] = max;
		}

		return;
	}

	Vector2
		** left = NULL,		// array of vectors, left of the intersection line
		** right = NULL;	// array of vectors, right of the intersection line

	int
		leftsz = 0,			// size of the left array
		rightsz = 0;		// size of the right array

	/*
	* Iterate over all vectors in data.
	*
	* Assign vectors to either the left or
	* right array depending on their cross product
	* with the min and max vectors.
	*/
	for (register int i = 0; i < arrsz; i++)
	{
		if (vec2equ(min, arr[i]) || vec2equ(farpoint, arr[i]) || vec2equ(max, arr[i]))
			continue;

		float cproduct = cprod(min, farpoint, arr[i]);

		if (cproduct >= 0)
		{
			left = realloc(left, sizeof(Vector2*) * (leftsz + 1));

			left[leftsz++] = arr[i];
		}

		cproduct = cprod(farpoint, max, arr[i]);

		// This is relative to the line formed by farpoint, max, and arr[i]
		// so still need to check if the cross product is to the left 
		// of that line.
		if (cproduct >= 0)
		{
			right = realloc(right, sizeof(Vector2*) * (rightsz + 1));

			right[rightsz++] = arr[i];
		}
	}

	// Recursively look for extremes in the left array.
	_dc_hull(hull, left, min, farpoint, leftsz);

	// Recursively look for extremes in the right array.
	_dc_hull(hull, right, farpoint, max, rightsz);

	if (left)
		free(left);

	if (right)
		free(right);
}
