#include <...>
using namespace std;

void writetofile(args) {
  open file

  for (int i=0; i<Nvertices; i++) {
    for (each adjacent vertex j) {
      pretty-print name[i] and name[j] 
  }

  close file
}

int main(args) {
  handle command line arguments

  read data from stdin
  create name-to-index map

  create do_know graph:
  vertex list: vector<string>
  edge list: vector< set<int> >

  writetofile("do_know.txt", ...)

  create might_know graph:
  edge list: vector< set<int> >

  writetofile("might_know.txt", ...)
}
