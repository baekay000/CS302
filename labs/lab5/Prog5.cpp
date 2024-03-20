#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

// template <typename T>
void writetofile(string filename, vector<string> vertices,
                 vector<set<int>> vertices_set) {
  int max_length = 0;

  // the length of the longest name is determined
  for (int i = 0; i < vertices.size(); i++) {
    max_length = max(max_length, (int)vertices[i].length());
  }

  ofstream fout(filename.c_str(), fstream::trunc);

  for (int i = 0; i < vertices.size(); i++) {
    int counter8 = 1;
    fout << left << setw(max_length) << vertices[i] << " : ";
    for (int mysetit : vertices_set[i]) {
      if (counter8 % 9 == 0) {
        fout <<  endl;
        fout << left << setw(max_length) << vertices[i] << " : ";
      }
      fout << setw(max_length) << vertices[mysetit] << " ";
      counter8++;
    }

    fout << endl;
  }
  fout.close();
}

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
  vector<pair<int, int>> adjacent_vertices;

  // vector of sets  --> do know and might know
  // vector<set<int>> do_know;

  // map to see if name exists
  map<string, int> name_map;

  // vertice pair using index ints for names
  vector<set<int>> edge_list;

  int index1 = 0;
  int index2 = 0;

  while (cin >> key1 >> key2) {
    name_map.insert(make_pair(key1, name_map.size()));
    name_map.insert(make_pair(key2, name_map.size()));
    adjacent_vertices.push_back(make_pair(name_map[key1], name_map[key2]));
  }

  edge_list.resize(name_map.size());

  for (int k = 0; k < (int)adjacent_vertices.size(); k++) {
    int i = adjacent_vertices[k].first;
    int j = adjacent_vertices[k].second;
    edge_list[i].insert(j);
    edge_list[j].insert(i);
  }

  vertices.resize(name_map.size());

  map<string, int>::iterator myit = name_map.begin();
  while (myit != name_map.end()) {
    vertices[myit->second] = myit->first;
    ++myit;
  }

  // writetofile("do_know.txt", ...)
  string fname_out = "doknow.txt";
  writetofile(fname_out, vertices, edge_list);

  vector<set<int>> friends_of_friends;
  friends_of_friends.resize(name_map.size());

  int myvertice = 0;
  for (set<int> setlist : edge_list) {
    for (int myfriend : setlist) {
      for (int potentialfriend : edge_list[myfriend]) {
        if (setlist.count(potentialfriend) == 0 && potentialfriend != myvertice) {
          friends_of_friends[myvertice].insert(potentialfriend);
        }
      }
    }
    myvertice++;
  }

  fname_out = "mightknow.txt";
  writetofile(fname_out, vertices, friends_of_friends);

  // template <typename T>
  // vector<set<T>> mytemplateset;


  // while (p1 != p2) {
  //   fout << *p1 << "\n";
  //   ++p1;
  // }
}

// create might_know graph : edge list
// : vector<set<int>>

// writetofile("might_know.txt", ...)
