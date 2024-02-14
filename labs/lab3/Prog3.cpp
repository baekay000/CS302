// include header files needed
using namespace std;

#include "Support.h"

// add omega() function from class05_sorting_4_handout.pdf

class data {
  // copy from Lab 2
  // add private ID integer variable
};

bool data::operator<(const data &rhs) const {
  // copy from Lab 2
  // remove phone number tiebreaker
} 

istream & operator>>(istream &in, data &r) {
  // copy from Lab 2
  // add static ID integer variable
  // increment and assign value to r.ID
}

ostream & operator<<(ostream &out, const data &r) {
  // copy from Lab 2
  // add r.ID output (setw(8), right justified)
}

template <typename T>
void printdata(T p1, T p2, string &fname) {
  // copy from Lab 2
}

  // compare lastname, firstname, phone number
}

int main(int argc, char *argv[]) {
  // error check command line arguments
  // CS302: Prog3 -insertion|mergesort|stdsort file.txt
  // CS307: Prog3 -insertion|mergesort|stdsort|qsort_r3 file.txt

  set mode
  set fname_in
  set fname_out

  open file.txt 

  dlist<data> A;

  data din;
  while (fin >> din)
    A.push_back(din);

  close file.txt
  
  // T0 = omega()

  if (mode is insertion) {
    A.insertion_sort();
  } else 
  if (mode is mergesort) {
    A.mergesort();
  } else 
  if (mode is stdsort) {
    A.quicksort(1);
  } else 
  if (mode is qsort_r3) {
    A.quicksort(2);
  }

  // T1 = omega()
  // print T1-T0 in ms

  printdata(A.begin(), A.end(), fname_out);
}
