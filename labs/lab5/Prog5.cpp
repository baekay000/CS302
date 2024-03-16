#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void writetofile(string filename) {
  // open file
  ifstream fstream{filename};

  for (int i = 0; i < Nvertices; i++) {
    // for (each adjacent vertex j) {
    // pretty - print name[i] and name[j]
  }

  // close file
  fstream.close();
}

// void graph<Tkey>::read(const char *fname) {
//   ifstream in(fname);
//   string input;
//   getline(in, input);
//   if (input.compare("# UNDIRECTED") == 0)
//     graph_type = UNDIRECTED;
//   else if (input.compare("# DIRECTED") == 0)
//     graph_type = DIRECTED;
//   else {
//     cerr << "error: graph type not known\n";
//     exit(1);
//   }
//   // Create mapping from key to index
//   Tkey key1, key2;
//   vector<pair<int, int>> Eij;
//   while (in >> key1 >> key2) {
//     key_map.insert(make_pair(key1, key_map.size()));
//     key_map.insert(make_pair(key2, key_map.size()));
//     Eij.push_back(make_pair(key_map[key1], key_map[key2]));
//   }
//   in.close();

  int main(int argc, char *argv[]) {
    bool sort = false;

    // handle command line arguments

    if (argc != 1 && argc != 2) {
      cerr << "usage: ./sprog5 [-sort] < file.txt" << endl;
      exit(1);
    }
    if (argc == 2 && (string)argv[1] != "-sort") {
      cerr << "usage: ./sprog5 [-sort] < file.txt" << endl;
      exit(1);
    } else if (argc == 2) {
      sort = true;
    }

    // read data from stdin create name - to -
    // index map

    // create do_know graph : vertex list : vector<string> edge list
    // : vector<set<int>>

    // writetofile("do_know.txt", ...)

    // create might_know graph : edge list
    // : vector<set<int>>

    // writetofile("might_know.txt", ...)
  }
