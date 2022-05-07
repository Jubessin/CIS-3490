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

int _mergeSort(int* arr, int* temp, int left, int right);
int merge(int* arr, int* temp, int left, int mid, int right);

int dc_inversion(int* arr)
/*
* Global function, called from main.
* 
* Simply creates a temporary array, used in breaking and
* resorting elements of original array, before
* calling the recursive _mergeSort function.
* 
* Returns the total number of inversions.
*/
{
	// temporary array.
	int* temp = (int*)malloc(sizeof(int) * P1_SIZE);

	int total = _mergeSort(arr, temp, 0, P1_SIZE - 1);

	free(temp);

	return total;
}

int _mergeSort(int* arr, int* tmp, int l, int r)
/*
* Local function.
* 
* Recursively divides an array until left with only 
* one element, then rejoins the array, counting the number
* of inversions during the rejoining of both left, right,
* and middle arrays.
* 
*/
{
	int invs = 0;	// number of inversions

	// Continue to divide the array into smaller chunks
	// until only working with one element.
	if (r > l) 
	{
		// Get the middle of the current chunk.
		int m = (r + l) * 0.5;

		// Continue to do this until the left "branch" has been
		// fully divided.
		invs += _mergeSort(arr, tmp, l, m);

		// Continue to do this until the right "branch" has been
		// fully divided.
		invs += _mergeSort(arr, tmp, m + 1, r);

		// Add the inversions that result from merging.
		invs += merge(arr, tmp, l, m + 1, r);
	}
	return invs;
}

int merge(int* arr, int* tmp, int l, int m, int r)
/*
* Local function.
* 
* Merges the array into the temporary array,
* incrementing the number of inversions whenever
* an element from the right array is merged before
* one from the left.
*/
{
	int 
		i = l,		// index for left array.
		j = m,		// index for right array (starts at the middle).
		k = l,		// index for merged array.
		invs = 0;	// Number of inversions.

	// Iterate until either indices exceed the bounds
	// of their associated array.
	while ((i <= m - 1) && (j <= r)) 
	{
		// Check if the lowest element from the left array is 
		// less or vec2equ to the lowest element in the right array. 
		if (arr[i] <= arr[j]) 
		{
			// Add the lowest element from the left array to 
			// the sorted array, and increment left array index.
			tmp[k] = arr[i++];
		}
		else 
		{
			// Add the lowest element from the right array to 
			// the sorted array, and increment right array index.
			tmp[k] = arr[j++];

			/*
			* The element at index i is current lowest value in the left array 
			* that has not been added to the sorted array. 
			*
			* The index m is the middle of the entire array.
			* 
			* If there was an element x in the right array that was
			* lesser than an element at index i in the left array, it would stand to reason
			* that x would also be smaller than all other elements following index i 
			* in the left array.
			* 
			* In this way, each of those elements that are greater than the element
			* at index i (including the element at index i) represent an inversion
			* that can be found by (m - i).
			*/
			invs += (m - i);
		}

		// An element will always be added to the new,
		// sorted array.
		++k;
	}

	// Add all remaining elements in the left array.
	while (i <= m - 1)
		tmp[k++] = arr[i++];

	// Add all remaining elements in the right array.
	while (j <= r)
		tmp[k++] = arr[j++];

	// Copy tmp array to the original array.
	for (i = l; i <= r; i++)
		arr[i] = tmp[i];

	return invs;
}
