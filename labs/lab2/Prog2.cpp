/* Kaylee Bae
Lab 2: Program that uses standard sort, quicksort, and quickselect functions to
sort a list of names and phone numbers.
*/

#include "Support.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

class data {
  friend istream &operator>>(istream &, data &);
  friend ostream &operator<<(ostream &, const data &);

public:
  bool operator<(const data &) const;

private:
  string firstname;
  string lastname;
  string phonenum;
};

// compare lastname, firstname, phone number
bool data::operator<(const data &rhs) const {
  if (lastname == rhs.lastname) {
    if (firstname == rhs.firstname) {
      // compare phone numbers last
      return phonenum < rhs.phonenum;

    } else {
      return firstname < rhs.firstname;
    }

  } else {
    return lastname < rhs.lastname;
  }
}

// input operator reads data object data
istream &operator>>(istream &in, data &r) {
  in >> r.firstname >> r.lastname >> r.phonenum; 
  return in;
}

// write this to write data object data
ostream &operator<<(ostream &out, const data &r) {
  string name = r.lastname + ',' + r.firstname;
  out << left << setw(23) << setfill('.') << name << " " << right << r.phonenum;
  return out;
}

template <typename T> void printdata(T p1, T p2, string &fname) {
  ofstream fout(fname.c_str(), fstream::trunc);

  while (p1 != p2) {
    fout << *p1 << "\n";
    ++p1;
  }

  fout.close();
}

int main(int argc, char *argv[]) {
  bool stdsort = false;

  // command line argument handling
  if (argc != 3 && argc != 5) {
    cerr << "usage: ./Prog2 -stdsort | -qsort [k0 k1] file.txt" << endl;
    return 1;
  }

  // argv[1] must be -stdsort or -qsort
  if (argc == 3) {
    if ((string)argv[1] != "-stdsort" && (string)argv[1] != "-qsort") {
      cerr << "usage: ./Prog2 -stdsort | -qsort [k0 k1] file.txt" << endl;
      return 1;
    }
    // if command line argument is std sort
    if ((string)argv[1] == "-stdsort") {
      // set stdsort to true
      stdsort = true;
    }
  }

  // set fname_in to the file user inputs and read it
  string fname_in = argv[argc - 1];
  ifstream fin(fname_in);
  string line;
  string fname_out;

  vector<data> A;

  // Store in data into a vector using overloaded operator
  data din;
  while (fin >> din) {
    A.push_back(din);
  }

  // Close fin
  fin.close();

  // Srand will seed fake 'random numbers'
  srand(302);

  // Implement the 3 types of sorts
  if (stdsort) {
    sort(A.begin(), A.end());
    fname_out = "stdsort_" + fname_in;

  } else {

    // if qsort
    int N = (int)A.size();
    fname_out = "qsort_" + fname_in;

    int k0 = 0;
    int k1 = N - 1;

    // V3, update k0, k1 and apply quickselect
    if ((string)argv[argc - 2] != "-qsort") {

      fname_out = "qselect_" + fname_in;

      // partition the input data -> k1 and k2
      k0 = stoi(argv[argc - 3]); // (string)
      if (k0 < 0) {
        k0 = 0;
      }

      k1 = stoi(argv[argc - 2]);
      if (k1 >= N) {
        k1 = N - 1;
      }

      // implement quickselect 2 times

      //k0-th element in its proper location
      quickselect(A, 0, k0, N - 1);

      //k1-th element in its proper location
      quickselect(A, k0, k1, N - 1);
    }

    // Quick sort is applied for both quickselect and quicksort
    quicksort(A, k0, k1);
  }

  // Prints out data
  printdata(A.begin(), A.end(), fname_out);
}
