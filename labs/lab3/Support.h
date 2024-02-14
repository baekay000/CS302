#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <algorithm>
#include <vector>

template <typename T>
class sptr {
  // copy from class06_sorting5_handout.pdf
};

// CS307 -----------------------------------------
template <typename T>
void qsort_r3(vector<T> &A, int left, int right) {
  // modify random pivot quicksort from Lab 2 to 
  // partition A into sublists {<}, {pivot,=}, {>} 
}

template <typename T>
void qsort_r3(vector<T> &A) {
  qsort_r3(A, 0, A.size()-1);
}

template <typename T>
class dlist { 
  // copy from class06_sorting5_handout.pdf
  // provide node with a less-than operator
  // add public quicksort() member function
}

template <typename T>
struct dlist<T>::node *dlist<T>::merge(...) {
  // write this based on HW5
}

template <typename T>
void dlist<T>::quicksort(int mode) {
  // determine length of linked list
  // create vector of smart pointers
  // CS302: : apply std::sort() 
  // CS307 mode=1: apply std::sort() 
  // CS307 mode=2: apply qsort_r3()
  // reorder linked list 
}

#endif // __SUPPORT_H__
