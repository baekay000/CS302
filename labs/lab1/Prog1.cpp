/* Kaylee Bae
Lab 1: Program that alphabetizes a handful of cities and states and their
weather data using various data structures from CS202.
*/

#include "Support.h"
#include <cstdio>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {

  bool rawdata = false;
  string userinput;

  // command line argument handling
  if (argc != 2 && argc != 3) {
    cerr << "usage: ./Prog1 [-rawdata] datafile" << endl;
    return 1;
  }

  // raw data must be argv[1] if there are 3 command line arguments
  if (argc == 3) {
    if ((string)argv[1] != "-rawdata") {
      cerr << "usage: ./Prog1 [-rawdata] datafile" << endl;
      return 1;
    }
    rawdata = true;
  }

  database n_database;
  string filename = argv[argc - 1];

  // init all raw data
  n_database.init_rawdata(filename);

  // if summary should be printed
  if (!rawdata) {
    n_database.init_summary();
    // infinite user input
    while (cin) {
      cout << "print> ";
      cin >> userinput;
      // EOF to cause a break
      if (cin.eof()) {
        break;
      }

      // print summary for user input
      n_database.print_summary(userinput);
    }
    cout << endl;

  } else {
    // if -rawdata, then print the raw data
    n_database.print_rawdata();
  }

  return 0;
}