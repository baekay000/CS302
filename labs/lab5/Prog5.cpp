#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

// void writetofile(string filename) {
//   // open file
//   ifstream fstream{filename};

//   for (int i = 0; i < Nvertices; i++) {
//     // for (each adjacent vertex j) {
//     // pretty - print name[i] and name[j]
//   }

//   // close file
//   fstream.close();
// }

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

  string key1, key2;
  // pair<string, string> mypair;
  // // map m where strings are the names and int is the order read
  // map<set<pair<string, string>>> m;

  // vector to store vertices that represent names read
  vector<string> vertices;

  // vector of sets (binary search trees) stores the indices of adjacent
  // vertices
  vector<set<int>> adjacent_vertices;

  // map to see if name exists
  map<string, int> name_map;

  // vertice pair using index ints for names
  set<int, int> vertice_pairs;

  int index1 = 0;
  int index2 = 0;

  while (cin >> key1 >> key2) {
    map<string, int>::iterator findkey1 = name_map.find(key1);
    map<string, int>::iterator findkey2 = name_map.find(key2);

    if (findkey1 != name_map.end()) {
      // name does not exist
      vertices.push_back(key1);
      index1 = vertices.size();
    } else {
      index1 = findkey1->second;
    }

    // unsure if necessary
    if (findkey2 != name_map.end()) {
      // name does not exist
      vertices.push_back(key2);
      index2 = vertices.size();
    } else {
      index2 = findkey1->second;
    }

    // insert int into set
    adjacent_vertices[index1].insert(index2);

    // for the other one two
    // adjacent_vertices[index1].insert(index2);

  }

  // create do_know graph : vertex list : vector<string> edge list
  // : vector<set<int>>

  // writetofile("do_know.txt", ...)
  string fname_out = "doknow.txt";

  ofstream fout(fname_out.c_str(), fstream::trunc);
  for (int i = 0; i < adjacent_vertices.size(); i++) {
    fout << setw(1) << vertices[i] << " : ";
    
    // iterate through set to print out all known people
    for (set<int>::iterator it = adjacent_vertices[i].begin(); it != adjacent_vertices[i].end(); ++it) {
      fout << *it << " ";
    }
    
    fout << endl;


  }

  // while (p1 != p2) {
  //   fout << *p1 << "\n";
  //   ++p1;
  // }

  fout.close();
}

// create might_know graph : edge list
// : vector<set<int>>

// writetofile("might_know.txt", ...)

