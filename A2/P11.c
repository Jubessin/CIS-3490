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

int bf_inversion(int* arr)
/*
* Global function, called from main.
* 
* Creates two loops, checking in the inner loop
* for whether the element in the inner loop is lower
* than the one of the outer loop.
*/
{
	int invs = 0;

	for (register int i = 0; i < P1_SIZE - 1; ++i)
		for (register int j = i + 1; j < P1_SIZE; ++j)
			if (arr[i] > arr[j])
				++invs;

	return invs;
}
