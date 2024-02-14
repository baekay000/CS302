/* Kaylee Bae
Lab 1: Program that alphabetizes a handful of cities and states and their
weather data using various data structures from CS202.
*/

#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

// For version 2
template <typename T> void quicksort(vector<T> &A, int left, int right) {

  // Copied code from Dr. Gregor with use of a randomly selected index
  int randomindex;

  if (left < right) {
    // Index can be any number from [left,right]
    randomindex = rand() % (right - left + 1) + left;

    T pivot = A[randomindex];
    // partition A: {<=}, {pivot}, {=>}
    swap(A[randomindex], A[right]);

    // Since the left and right are not sorted, we must begin and end a little
    // differently than in the original code
    int i = left - 1;
    int j = right;
    while (1) {
      while (A[++i] < pivot) {
      }
      while (pivot < A[--j]) {
      }
      if (i >= j)
        break;
      swap(A[i], A[j]);
    }
    randomindex = i;
    swap(A[randomindex], A[right]);

    // Quicksort both sides through recursion
    quicksort(A, left, randomindex - 1);
    quicksort(A, randomindex + 1, right);
  }
}

// For version 3 using k0 and k1
template <typename T>
void quickselect(vector<T> &A, int left, int k, int right) {

  while (1) {
    // same randomindex implementation
    int randomindex = rand() % (right - left + 1) + left;

    T pivot = A[randomindex];
    // partition A: {<=}, {pivot}, {=>}
    swap(A[randomindex], A[right]);

    // Same logic [left, right]
    int i = left - 1;
    int j = right;

    // Since this is preincrement, we need to make sure that i = left -1 and j =
    // right so the first item we check is A[left] to A[right -1]
    while (1) {
      while (A[++i] < pivot) {
      }
      while (pivot < A[--j]) {
      }
      if (i >= j)
        break;
      swap(A[i], A[j]);

    }
    randomindex = i;
    swap(A[randomindex], A[right]);
    // return pindex;

    // stop partition

    if (randomindex == k)
      return;
    if (k < randomindex)
      right = randomindex - 1;
    else
      left = randomindex + 1;
  }
}

#endif // __SUPPORT_H__
