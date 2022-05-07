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

Vector2** order(Hull*);

Hull* bf_hull(Vector2** data)
/*
* Global function, called from main.
* 
* An initial loop is first created. There is a
* check to make sure that the vector at the current
* index has not been added to the hull already.
* 
* Another loop is then created within the first loop
* in order to check pairs of vectors.
* 
* An additional, third loop, is created where at each
* index, not equal to the current one in the first and second loop,
* the cross product of the vector is checked.
* 
* If the cross product is positive, the current vector lies inside the hull,
* and the current vector is invalidated.
* 
* If the cross product is zero, there is a distance check with 
* first vector - second vector and first vector - third vector.
* And the vector closer to the first vector is added to a list of colinears
* 
* If all vectors lie to the right of the second vector it is validated,
* the first vector is added to the hull and the second vector is checked
* for whether it already exists in the hull before it is also added
* alongside its colinears.
* 
* Lastly, when all loops are finished, the vectors within the hull must
* be ordered before returning.
* 
*/
{
	Hull* hull = malloc(sizeof(Hull));
	
	Vector2 **colinears = NULL;	// list of colinears
	Vector2 
		*vec2i = NULL,			// vector corresponding to outermost loop
		*vec2j = NULL;			// vector corresponding to middle (2nd) loop

	int colsz = 0;				// size of colinears array

	bool allr = true;			// check if all vectors lie to the right

	/* Initialize the hull variables. */
	hull->points = NULL;
	hull->sz = 0;

	/* First loop */
	for (register int i = 0; i < P2_SIZE; ++i)
	{
		// Check if vector already exists in the hull.
		if (contains(hull->points, data[i], hull->sz))
			continue;

		vec2i = data[i];

		/* Second loop */
		for (register int j = 0; j < P2_SIZE; ++j)
		{
			if (i == j)
				continue;

			// Reset size of colinears array.
			colsz = 0;

			vec2j = data[j];

			// Reset boolean.
			allr = true;

			/* Third loop */
			for (register int k = 0; k < P2_SIZE; ++k)
			{
				if (j == k || i == k)
					continue;

				// Get the cross product of i, j, and k vectors.
				int cproduct = cprod(vec2i, vec2j, data[k]);

				// There exists a vector that is further left
				// than the j vector.
				if (cproduct > 0)
				{
					colsz = 0;
					allr = false;
					break;
				}

				// There exists a vector that is colinear 
				// with the j vector.
				if (cproduct == 0)
				{
					colinears = realloc(colinears, sizeof(Vector2*) * (colsz + 1));
					
					// Compare the distances of the j and k vectors
					// to the i vector.
					if (dist(vec2i, vec2j) < dist(vec2i, data[k]))
					{
						colinears[colsz++] = vec2j;
						
						vec2j = data[k++];
					}
					else
					{
						colinears[colsz++] = data[k++];
					}
				}
			}

			// The j vector is an extreme point, no other
			// vectors lie to its left.
			if (allr)
			{
				/* Always add the i vector. */
				hull->points = realloc(hull->points, sizeof(Vector2*) * (hull->sz + colsz + 2));

				hull->points[hull->sz++] = vec2i;

				// Check whether the j vector has been added to avoid duplicates.
				if (contains(hull->points, vec2j, hull->sz))
					continue;

				// Add colinears before the j vector.
				for (register int k = 0; k < colsz; k++)
					hull->points[hull->sz++] = colinears[k];

				hull->points[hull->sz++] = vec2j;
			}
		}
	}

	if (colinears)
		free(colinears);

	// Get the ordered vectors of the hull.
	Vector2** ordered = order(hull);

	free(hull->points);

	hull->points = ordered;

	return hull;
}

Vector2** order(Hull* hull)
/*
* Local function.
* 
* Orders, and returns the vectors of the hull.
* 
* Functions similarly to a mixture of Jarvis' March/Graham Scan
* algorithms, and could also be used to find the extremes of the 
* convex hull.
*/
{
	if (hull->sz == 0)
		return NULL;

	Vector2 
		**vectors = NULL,	// ordered list of vectors
		**colinears = NULL;	// list of colinears

	Vector2
		*min = NULL,	// minimum x Vector2
		*prev = NULL;	// last added Vector2 on hull

	int 
		colsz = 0,		// number of colinears
		tmpsz = 0;		// index for vectors array
	
	vectors = malloc(sizeof(Vector2*));
	colinears = malloc(sizeof(Vector2*));

	// Get the vector with the minimum x value.
	vectors[tmpsz++] = prev = min = minx(hull->points, hull->sz);

	/* Infinitely loop */
	while (1)
	{
		Vector2* next = NULL;	// next potential vector to be added
		colsz = 0;				// reset the size of colinear array

		/* Iterate over all vectors in hull. */
		for (register int i = 0; i < hull->sz; i++)
		{
			// Skip over previous vector found.
			if (vec2equ(prev, hull->points[i]))
			{
				continue;
			}
			if (next == NULL)
			{
				next = hull->points[i];
				continue;
			}

			// Get the cross product of the previous, (potential) next, and current vectors.
			float cproduct = cprod(prev, next, hull->points[i]);

			// Positive cross product indicates that
			// the current point lies to the left of the 
			// (potential) next point.
			if (cproduct == 1)
			{
				if (colinears)
					free(colinears);

				colinears = NULL;		// Reset colinear array
				colsz = 0;				// Reset size of colinear array

				next = hull->points[i];	// Assign new next
			}
			// Zero cross product indicate colinearity
			else if (cproduct == 0)
			{
				// Create space for another colinear vector
				colinears = realloc(colinears, sizeof(Vector2*) * colsz + 1);

				if (dist(prev, next) < dist(prev, hull->points[i]))
				{
					// add next to colinears
					colinears[colsz++] = next;

					next = hull->points[i];
				}
				else
				{
					// add data[i] to colinears
					colinears[colsz++] = hull->points[i];
				}
			}
		}

		// If the next potential vector is the minimum
		// vector then all vectors on the hull have been found.
		if (vec2equ(min, next))
			break;

		// Check if there are any colinears
		if (colsz > 0)
		{
			// Resize vectors list to incorporate colinears.
			vectors = realloc(vectors, sizeof(Vector2*) * (tmpsz + colsz));

			/* Add all colinears to ordered vector list. */
			for (register int i = 0; i < colsz; i++)
			{
				vectors[tmpsz + i] = colinears[i];
				++tmpsz;
			}

			if (colinears)
				free(colinears);

			colsz = 0;
		}

		// add next to hull
		vectors = realloc(vectors, sizeof(Vector2*) * (tmpsz + 1));
		vectors[tmpsz++] = next;

		prev = next;
	}

	if (colinears)
		free(colinears);

	return vectors;
}
