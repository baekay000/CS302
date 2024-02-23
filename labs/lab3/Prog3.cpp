/* Kaylee Bae
Lab 3: Program that uses various sorting functions to sort a list of names and
phone numbers.
*/

// header files
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

#include "Support.h"

class data {
  // copy from Lab 2
  friend istream &operator>>(istream &, data &);
  friend ostream &operator<<(ostream &, const data &);

public:
  bool operator<(const data &) const;

private:
  string firstname;
  string lastname;
  string phonenum;

  // ID integer variable showing position of the data entry in input file
  int id;
};

// Compares data
bool data::operator<(const data &rhs) const {

  // copy from Lab 2 without phone number tiebreaker
  if (lastname == rhs.lastname) {
    return firstname < rhs.firstname;

  } else {
    return lastname < rhs.lastname;
  }
}

// Input stream to take in names and phone numbers
istream &operator>>(istream &in, data &r) {
  // copy from Lab 2
  in >> r.firstname >> r.lastname >> r.phonenum;

  // add static ID integer variable
  static int id = 1;

  // increment and assign value to r.ID
  r.id = ++id;

  return in;
}

// output data with r.id
ostream &operator<<(ostream &out, const data &r) {
  // copy from Lab 2
  string name = r.lastname + ',' + r.firstname;

  // add r.ID output with ' ' as fill
  out << left << setw(23) << setfill('.') << name << " " << right << r.phonenum
      << right << setfill(' ') << setw(9) << r.id;
  return out;
}

template <typename T> void printdata(T p1, T p2, string &fname) {
  // copy from Lab 2
  ofstream fout(fname.c_str(), fstream::trunc);

  while (p1 != p2) {
    fout << *p1 << "\n";
    ++p1;
  }

  fout.close();
}

int main(int argc, char *argv[]) {
  bool insertion = false;
  bool mergesort = false;
  bool stdsort = false;
  string fname_out;

  // error check command line arguments
  if (argc != 3) {
    cerr << "usage: ./Prog3 -insertion|mergesort|stdsort|quicksort file.txt"
         << endl;
    return 1;
  }

  // make sure that argv[1] is one of the 3 sorting algorithms
  if ((string)argv[1] != "-insertion" && (string)argv[1] != "-mergesort" &&
      (string)argv[1] != "-stdsort") {
    cerr << "usage: ./Prog3 -insertion|mergesort|stdsort|quicksort file.txt"
         << endl;
    return 1;
  }

  // read file
  string fname_in = argv[argc - 1];

  // open file.txt
  ifstream fin(fname_in);

  dlist<data> A;
  data din;

  // push data into A
  while (fin >> din) {
    A.push_back(din);
  }

  // Close fin
  fin.close();

  // extract which mode
  if ((string)argv[1] == "-insertion") {
    insertion = true;
    fname_out = "insertion_" + fname_in;

  } else if ((string)argv[1] == "-mergesort") {
    mergesort = true;
    fname_out = "mergesort_" + fname_in;

  } else if ((string)argv[1] == "-stdsort") {
    stdsort = true;
    fname_out = "stdsort_" + fname_in;
  }

  // Perform listed sort
  if (insertion) {
    A.insertion_sort();
  } else if (mergesort) {
    A.mergesort();
  } else if (stdsort) {
    A.quicksort(1);
  }

  // Prints data into output files
  printdata(A.begin(), A.end(), fname_out);
}
