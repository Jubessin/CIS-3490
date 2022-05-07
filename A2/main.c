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

#define DELIM " \n\r"	// delimiter for tokenizing data from file

void getIntegers(int*, const char*, const int);
void getPoints(Vector2**, const char*, const int);

void runP1(int (*func)(int*), int*);
void runP2(Hull* (*func)(Vector2**), Vector2**);

float getPath(Hull*, Hull*, Vector2*, Vector2*);

int main(int argc, char* argv[])
/*
* Main function.
* 
* Delegates tests of algorithms to runP1, 
* and runP2 functions.
*/
{
	int* p1_data = NULL;		// array for data
	Vector2** p2_data = NULL;	// array for data

	p1_data = malloc(sizeof(int) * P1_SIZE);
	p2_data = malloc(sizeof(Vector2*) * P2_SIZE);

	// Check that memory was allocated properly
	if (!p1_data || !p2_data)
	{
		fprintf(stderr, "Error allocating memory.\n");
		exit(1);
	}

	// Get the data for P1.1 and P1.2
	getIntegers(p1_data, P1_FILE, P1_SIZE);

	// Get the data for P2.1 and P2.2
	getPoints(p2_data, P2_FILE, P2_SIZE);

	printf("\nP1.1 - Brute Force Program for counting inversions -\n");

	runP1(&bf_inversion, p1_data);

	printf("\nP1.2 - Divide and Conquer Program for counting inversions -\n");

	runP1(&dc_inversion, p1_data);

	printf("\nP2.1 - Brute Force Program for computing hull -\n");

	runP2(&bf_hull, p2_data);

	printf("\nP2.2 - Divide and Conquer Program for computing hull -\n");

	runP2(&dc_hull, p2_data);

	// Free allocated memory.
	if (p1_data)
	{
		free(p1_data);
	}
	if (p2_data)
	{
		for (int i = 0; i < P2_SIZE; i++)
		{
			if (p2_data[i])
				free(p2_data[i]);
		}

		free(p2_data);
	}
	
	return 0;
}

void getIntegers(int* arr, const char* filename, const int size)
{
	// Open file with read access.
	FILE* fp = fopen(filename, "r");

	if (!fp)
		return;

	int i = 0;			// index in array
	size_t bufsz;		// size of the buffer, reallocated as needed by getline.

	char *buf = NULL, *token = NULL;

	// Read file line by line.
	while ((getline(&buf, &bufsz, fp)) != EOF)
	{
		if (i >= size)
			break;

		// tokenize input
		token = strtok(buf, DELIM);

		/* Tokenize line until NULL */
		while (token)
		{
			arr[i++] = atoi(token);		// Get the integer from the space-separated token.
			token = strtok(NULL, DELIM);
		}
	}

	fclose(fp);

	if (buf)
		free(buf);
}

void getPoints(Vector2** arr, const char* filename, const int pairs)
{
	// Open the file with read access.
	FILE* fp = fopen(filename, "r");

	if (!fp)
		return;

	int i = 0;		// index of the array
	float x, y;		// component of the line

	/* Attempt to read two floats from each line of the file. */
	while ((fscanf(fp, "%f %f", &x, &y)) == 2)
	{
		// Check that the index does not exceed array bounds.
		if (i >= pairs)
			break;

		// Allocate and initialize new Vector2 structure.
		arr[i] = malloc(sizeof(Vector2));
		arr[i]->x = x;
		arr[i++]->y = y;
	}

	fclose(fp);
}

void runP1(int (*func)(int*), int* data)
/*
* Local function, called by main.
* 
* Determines the processing time of the passed function pointer,
* in milliseconds.
* 
* Displays the result retrieved from the function, as well as the
* execution time.
*/
{
	clock_t start, stop;	// used for timing the function.

	int count;				// function return value

	start = clock();		// get the current time

	count = func(data);

	stop = clock();			// get the current time (after function execution)

	printf("Count = %d\n", count);		// display result from function

	printf("Execution time = %.0f ms\n", (((double)(stop - start) / CLOCKS_PER_SEC)) * 1000);	// display execution time
}

void runP2(Hull* (*func)(Vector2**), Vector2** data)
/*
* Local function, called by main.
*
* Determines the processing time of the passed function pointer,
* in milliseconds.
*
* Displays the execution time of the function.
* 
* Prompts user for input, in calculating the shortest path.
* 
* If invalid input is received (e.g., one or more vectors not found on the extremes
* of the convex hull) a distance of 0 is displayed, and no vectors are
* displayed to the screen.
* 
* Utilizes the getPath function for finding the shortest path.
*/
{
	clock_t start, stop;	// used for timing the function.

	Hull
		*hull = NULL,		// the convex hull
		*path = NULL;		// the shortest path

	Vector2
		*s1 = NULL,			// vector input from user
		*s2 = NULL;			// vector input from user

	float distance;			// distance of shortest path
	
	start = clock();		// get current time

	hull = func(data);

	stop = clock();			// get current time

	printf("Time for computing hull: %.0f ms\n", (((double)(stop - start) / CLOCKS_PER_SEC)) * 1000);	// display execution time

	/* Allocate and initialize variable for path. */
	path = malloc(sizeof(Hull));
	path->points = NULL;
	path->sz = 0;

	/* Allocate vector structures for user input. */
	s1 = malloc(sizeof(Vector2));
	s2 = malloc(sizeof(Vector2));

	// Get user input for start vector.
	printf("Enter the x and y coordinates of s1: ");
	scanf("%f %f", &s1->x, &s1->y);

	// Get user input for stop vector.
	printf("Enter the x and y coordinates of s2: ");
	scanf("%f %f", &s2->x, &s2->y);

	// Attempt to get the shortest path, and distance.
	distance = getPath(hull, path, s1, s2);

	printf("The number of points on the shortest path: %d\n", path->sz);

	printf("The distance: %.1f\n", distance);

	printf("The points on the path:\n");

	for (int i = 0; i < path->sz; i++)
		printf("(%.1f, %.1f)\n", path->points[i]->x, path->points[i]->y);

	free(s1);
	free(s2);

	if (path->points)
		free(path->points);

	free(path);

	free(hull->points);
	free(hull);
}

float getPath(Hull* hull, Hull* path, Vector2* s1, Vector2* s2)
/*
* Local function, called by main.
* 
* Gets the shortest path from s1 to s2, if possible, and 
* returns the distance.
*/
{
	if (vec2equ(s1, s2))
		return 0;

	float 
		dist_between = 0,	// distance between s1 and s2 (in forward array direction) 
		dist_outside = 0;	// distance outside of s1 and s2 (reverse direction)

	int 
		s1_in = -1,			// index of s1 in array 
		s2_in = -1;			// index of s2 in array

	bool between = false;	// flag for whether currently in between s1 and s2

	int i = 0;				// general index

	/* Loop infinitely. */
	while (1)
	{
		// If the index for s1 has not been found, compare with current vector.
		if (s1_in == -1 && vec2equ(hull->points[i], s1))
		{
			// Flip flag.
			between = !between;

			// Get the distance from last vector if index is 0.
			if (i == 0)
				dist_outside += dist(s1, hull->points[hull->sz - 1]);

			// Assign s1 index.
			s1_in = i;
		}

		// If the index for s2 has not been found, compare with current vector.
		if (s2_in == -1 && vec2equ(hull->points[i], s2))
		{
			// Flip flag.
			between = !between;

			// Get the distance from last vector if index is 0.
			if (i == 0)
				dist_outside += dist(s2, hull->points[hull->sz - 1]);

			// Assign s2 index.
			s2_in = i;
		}

		// Break if end of array reached.
		if (i == hull->sz - 1)
			break;

		// Add distance.
		if (between)
			dist_between += dist(hull->points[i], hull->points[i + 1]);
		else
			dist_outside += dist(hull->points[i], hull->points[i + 1]);

		++i;
	}

	// Check that an index was found for s1 and s2.
	if (s1_in == -1 || s2_in == -1)
	{
		path->sz = 0;
		return 0;
	}

	// Start loop at index of s1.
	i = s1_in;

	while (1)
	{
		// Reset the index to the beginning or end of the array
		// if a bound was reached.
		if (i == hull->sz)
			i = 0;
		else if (i == -1)
			i = hull->sz - 1;

		// Allocate space for an additional vector.
		path->points = realloc(path->points, sizeof(Vector2*) * (path->sz + 1));

		path->points[path->sz++] = hull->points[i];

		// Break the loop once the stop vector index is reached.
		if (i == s2_in)
			break;

		if (dist_between < dist_outside)
		{
			/*
			* Assume a total size was 22, and s1 was at index 4,
			* with s2 at index 14.
			* 
			* If the distance between s1 and s2 (indices 4 to 14) was lower
			* than the distance outside s1 and s2 (indices 4 to 0, indices 22 to 14)
			* then, as s1 index is lower, increment until index 14 is reached.
			* 
			* If s1 were to be at index 14 instead, while s2 were to be at index 4
			* then decrement until index 4 is reached.
			*/
			if (s1_in < s2_in)
				++i;
			else
				--i;
		}
		else
		{
			/*
			* Assume a total size was 22, and s1 was at index 1,
			* with s2 at index 21.
			*
			* If the distance outside s1 and s2 (indices 1 to 0, indices 22 to 21) was lower
			* than the distance outside s1 and s2 (indices 1 to 21)
			* then, with s1 index being lower, increment until index 21 is reached.
			*
			* If s1 were to be at index 21 instead, while s2 were to be at index 4
			* then increment until index 22 is reached, reset to index 0, and increment until
			* index 1 is reached.
			*/
			if (s1_in < s2_in)
				--i;
			else
				++i;
		}
	}

	// Return the lower of two distances.
	return (dist_between < dist_outside)
		? dist_between
		: dist_outside;
}
