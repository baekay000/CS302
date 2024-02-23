/*
Kaylee Bae
Lab 3: Defines node structs, data types, and functions necessary for sorting
algorithms in Prog3.cpp.
*/

#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <algorithm>
#include <cstddef>
#include <vector>

using namespace std;

// smart pointer
template <typename T> class sptr {
  // copy from class06_sorting5_handout.pdf
public:
  sptr(T *_ptr = NULL) { ptr = _ptr; }
  bool operator<(const sptr &rhs) const { return *ptr < *rhs.ptr; }
  operator T *() const { return ptr; }

private:
  T *ptr;
};

template <typename T> class dlist {
  // copy from class06_sorting5_handout.pdf

private:
  struct node {
    node() {
      key = T();
      prev = next = this;
    }
    node(T &n_key) {
      key = n_key;
      prev = next = this;
    }
    T key;
    node *prev;
    node *next;

    // Add overloaded less-than operator
    bool operator<(node &other) { return key < other.key; }
  };
  node *head;

public:
  dlist() {
    head = new node();
    // For CS302: when calling quicksort, mode is set to 1
    void quicksort(int mode);
  }
  ~dlist() {
    // Destructor to prevent memory leaks
    node *tmp = head->next;

    while (tmp != head) {
      tmp = tmp->next;
      delete tmp->prev;
    }
    delete tmp;
  }
  void push_back(T &);
  void insertion_sort();
  void mergesort();

  class iterator {
  public:
    iterator() { p = NULL; }
    T &operator*() { return p->key; }
    iterator operator++() {
      p = p->next;
      return *this;
    }
    bool operator!=(iterator &rhs) { return p != rhs.p; }

  private:
    iterator(node *np) { p = np; }
    node *p;
    friend class dlist<T>;
  };
  iterator begin() { return iterator(head->next); }
  iterator end() { return iterator(head); }

  // Add mergesort function and write merge function
  struct dlist<T>::node *mergesort(node *L, node *R);
  struct dlist<T>::node *merge(node *L, node *M, node *R);

  // add public quicksort() member function
  void quicksort(int mode);
};

// Copied
template <typename T> void dlist<T>::push_back(T &key) {
  node *p = new node(key);
  node *pp = head->prev;
  p->next = head;
  p->prev = pp;
  pp->next = p;
  head->prev = p;
}

// Copied
template <typename T> void dlist<T>::insertion_sort() {
  node *p = head->next;
  node *pp, *pn, *q;
  while (p != head) {
    pp = p->prev;
    pn = p->next;
    // unlink node p
    pp->next = pn;
    pn->prev = pp;
    // find node q preceeding node p
    q = pp;
    while (q != head && p->key < q->key)
      q = q->prev;
    // relink node p
    p->next = q->next;
    q->next = p;
    p->prev = q;
    p->next->prev = p;
    p = pn;
  }
}

// Copied
template <typename T> void dlist<T>::mergesort() {
  if (head->next == head->prev)
    return;
  head->next = mergesort(head->next, head);
}

// Copied
template <typename T>
struct dlist<T>::node *dlist<T>::mergesort(node *L, node *R) {
  if (L->next == R)
    return L;
  int N = 0;
  node *p = L;
  while (p != R) {
    p = p->next;
    N++;
  }
  node *M = L;
  for (int i = 0; i < N / 2; i++)
    M = M->next;
  L = mergesort(L, M);
  M = mergesort(M, R);
  L = merge(L, M, R);
  return L;
}

// Function that merges two sublists based on HW5
template <typename T>
struct dlist<T>::node *dlist<T>::merge(node *L, node *M, node *R) {
  node *H = L->prev; // sublist head node
  node *p = L;       // node to consider from sublist L
  node *q = M;       // node to consider from sublist M

  // Hold tmp variable to hold q
  node *tmp;

  // Merge stops when p == q or q ==R
  while (p != q && q != R) {
    tmp = q->next;

    // Compares key values to see if q has a smaller key
    if (q->key < p->key) {

      // Relink node so that q goes before p
      q->prev->next = q->next;
      q->next->prev = q->prev;
      p->prev->next = q;
      q->prev = p->prev;
      p->prev = q;
      q->next = p;

      // Move q back to the original q->next value
      q = tmp;
    } else {
      // Increment p
      p = p->next;
    }
  }

  return H->next;
}

// Uses std::sort for mode 1
template <typename T>
void dlist<T>::quicksort(int mode) {

  // determine length of linked list
  int length = 0;
  node *myit = head;
  while (myit->next != head) {
    length++;
    myit = myit->next;
  }

  // create vector of smart pointers
  vector<sptr<node>> Ap;
  node *nodeit = head;

  // iterate through the dlist to populate vector with correct node
  for (int i = 0; i < length; i++) {
    Ap.push_back(nodeit->next);
    nodeit = nodeit->next;
  }

  // CS302: : apply std::sort()
  node *sortit = head;
  std::sort(Ap.begin(), Ap.end());

  // Link up head node and the head->next node
  sortit->next = Ap[0];
  sortit->next->prev = head;
  sortit->next->next = Ap[1];

  // Start with head->next->next node for the 'for' loop
  sortit = sortit->next->next;

  // goes through all the elements and link up previous and next
  for (int i = 1; i < length; i++) {
    sortit = Ap[i];
    sortit->prev = Ap[i - 1];
    sortit->next = Ap[i + 1];
    sortit = sortit->next;
  }

  // special case for the end of the list, where the sorit->next should point to
  // head because it's a double linked list
  sortit = Ap[length - 1];
  sortit->prev = Ap[length - 2];
  sortit->next = head;
  sortit->next->prev = sortit;
}

#endif // __SUPPORT_H__
