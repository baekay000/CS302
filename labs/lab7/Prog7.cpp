/*
Kaylee Bae
NetID: kbae1
Recreates the Unix diff commands using the longest common subsequence algorithm.
User inputs two files and the diff commands are outputted in the terminal.
*/

#include <fstream>
#include <iostream>
#include <limits.h>
#include <set>
#include <string>
#include <vector>

using namespace std;

class diff_align {
public:
  diff_align() : DIAG(1), VERT(2), HORZ(4) {}

  // take in name of an input file and read into a vector containing string
  ifstream &load_data1(ifstream &);
  ifstream &load_data2(ifstream &);

  void compute_alignment();
  void print_alignment();

private:
  void print_edit(int, int, set<int>, set<int>);
  void print_alignment(int, int);

  vector<string> data1;
  vector<string> data2;

  // cost matrix link matrix trace list
  vector<vector<int>> cost; // edit costs
  vector<vector<int>> link; // edit operations
  vector<char> trace;       // edit path(s)

  // all copied from the handout
  const int DIAG;
  const int VERT;
  const int HORZ;

  int DEL(string c) { return 1; }
  int INS(string c) { return 1; }
  int SUB(string c1, string c2) { return c1 == c2 ? 0 : INT_MAX / 2; }

  int m, n;
};

// store each line in the input file into a vector holding strings
ifstream &diff_align::load_data1(ifstream &istrm) {
  string line;
  data1.push_back("-");
  while (getline(istrm, line)) {
    data1.push_back(line);
  }

  return istrm;
}

// strings represent the lines of text in the output file
ifstream &diff_align::load_data2(ifstream &istrm2) {
  string line;
  data2.push_back("-");
  while (getline(istrm2, line)) {
    data2.push_back(line);
  }

  return istrm2;
}

// computes the longest common subsequence alignment
void diff_align::compute_alignment() {

  m = data1.size() - 1; // number of lines of data1
  n = data2.size() - 1; // number of lines of data2

  // change to only look at ex as a string, not a char

  cost.assign(m + 1, vector<int>(n + 1));
  link.assign(m + 1, vector<int>(n + 1));

  // sets i=0,j=0 to 0
  cost[0][0] = 0;
  link[0][0] = 0;

  // iterate through the columns to fill in the costs and links
  for (int i = 1; i <= m; i++) {
    cost[i][0] = cost[i - 1][0] + DEL(data1[i]);
    link[i][0] = VERT;
  }

  // iterate through row 0
  for (int j = 1; j <= n; j++) {
    cost[0][j] = cost[0][j - 1] + INS(data2[j]);
    link[0][j] = HORZ;
  }

  // copied from Dr.Gregor's handout
  // fills in the cost and link matrix depending on shorter costs
  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      int delcost = cost[i - 1][j] + DEL(data1[i]);
      int subcost = cost[i - 1][j - 1] + SUB(data1[i], data2[j]);
      int inscost = cost[i][j - 1] + INS(data2[j]);

      // unless the two strings are matching, the cost and link will be replaced
      // with deletions or insertions
      cost[i][j] = subcost;
      link[i][j] = DIAG;
      if (delcost < cost[i][j]) {
        cost[i][j] = delcost;
        link[i][j] = VERT;
      }
      if (inscost < cost[i][j]) {
        cost[i][j] = inscost;
        link[i][j] = HORZ;
      }
    }
  }
}

// wrapper for a recursive private member function
void diff_align::print_alignment() { print_alignment(m, n); }

// uses the DIAG, VERT, HORZ link information set by compute_alignment()
void diff_align::print_alignment(int i, int j) {
  // this is the base case, when you are in the top left
  if (i == 0 && j == 0) {

    // delete and insert sets
    set<int> delete_s;
    set<int> insert_s;

    // go through the trace backwards
    for (int k = (int)trace.size() - 1; k >= 0; k--) {
      // if diagonal, call print_edit
      if (trace[k] == DIAG) {
        if (delete_s.size() != 0 || insert_s.size() != 0) {
          print_edit(i, j, delete_s, insert_s);
        }
        i++;
        j++;

        // clear sets
        delete_s.clear();
        insert_s.clear();
        continue;
      } else if (trace[k] == VERT) {
        i++;
        // this means delete
        delete_s.insert(i);

      } else {

        j++;
        // this means insert
        insert_s.insert(j);
      }
    }
    // call the print_edit at the end, in case the sequence did not end on a
    // match
    if (delete_s.size() != 0 || insert_s.size() != 0) {
      print_edit(i, j, delete_s, insert_s);
    }
    return;
  }

  // create the trace
  if (link[i][j] & DIAG) {
    trace.push_back(DIAG);
    print_alignment(i - 1, j - 1);
  } else if (link[i][j] & VERT) {
    trace.push_back(VERT);
    print_alignment(i - 1, j);
  } else if (link[i][j] & HORZ) {
    trace.push_back(HORZ);
    print_alignment(i, j - 1);
  }
}

// prints out the sequence of delete (d), add (a), and change (c)
void diff_align::print_edit(int i, int j, set<int> delete_s,
                            set<int> insert_s) {

  // check if change (c), meaning both sets are non empty
  if (!delete_s.empty() && !insert_s.empty()) {

    // print out the line from file1 to change
    if (delete_s.size() == 1) {
      cout << i;
    } else {
      // i - delete_s.size()+1 shows what lines were affected by the change
      cout << i - delete_s.size() + 1 << "," << i;
    }

    cout << "c";

    // print out line from file2 to change
    if (insert_s.size() == 1) {
      cout << j;
    } else {
      cout << j - insert_s.size() + 1 << "," << j;
    }
    cout << endl;

  } else if (!delete_s.empty()) {
    // deletion, since the deletion set in not empty

    if (delete_s.size() == 1) {
      cout << i;
    } else {
      cout << i - delete_s.size() + 1 << "," << i;
    }

    // deletion should not have more than one line affected in file2
    cout << "d" << j << endl;

  } else {
    // insertion since insertion set is not empty
    cout << i << "a";

    // insertion should have one or more lines inserted from file2
    if (insert_s.size() == 1) {
      cout << j;
    } else {
      cout << j - insert_s.size() + 1 << "," << j;
    }
    cout << endl;

  }

  // START printing out the lines themselves
  // deletion set
  for (int my_s : delete_s) {
    cout << "< " << data1[my_s] << endl;
  }

  // separation for changes
  if (!insert_s.empty() && !delete_s.empty()) {
    cout << "---" << endl;
  }

  // insertion set
  for (int my_s : insert_s) {
    cout << "> " << data2[my_s] << endl;
  }
}

int main(int argc, char *argv[]) {
  bool is_unix = false;
  bool is_verbose = false;

  // check that input files are specified on command line
  // parse commandline arguments for info, dist, and time
  if (argc != 3 && argc != 4 && argc != 5) {
    cerr << "usage: ./Prog7 [-unix] [-verbose] file1.txt file2.txt" << endl;
    return 1;
  }

  // check number of arguments provided
  if (argc > 3) {
    if (string(argv[1]) == "-unix" || string(argv[2]) == "-unix") {
      is_unix = true;
    }
    if (string(argv[1]) == "-verbose" || string(argv[2]) == "-verbose") {
      is_verbose = true;
    } else {
      cerr << "usage: ./Prog7 [-unix] [-verbose] file1.txt file2.txt" << endl;
      return 1;
    }
  }

  // get filenames
  string fname1 = argv[argc - 2];
  string fname2 = argv[argc - 1];

  // ensure they can be opened and if not, report and exit
  ifstream istream(fname1);
  ifstream istream2(fname2);

  if (!istream.is_open()) {
    cerr << "cannot open " << fname1 << endl;
    return 1;
  }
  if (!istream2.is_open()) {
    cerr << "cannot open " << fname2 << endl;
    return 1;
  }

  // instantiate diff_align object
  diff_align mydiff;

  mydiff.load_data1(istream);
  mydiff.load_data2(istream2);

  // execute diff_align::compute_alignment();
  mydiff.compute_alignment();

  // execute diff_align::print_alignment();
  mydiff.print_alignment();

  // close istreams
  istream.close();
  istream2.close();

  return 0;
}
