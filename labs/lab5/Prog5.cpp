/* Kaylee Bae
NetID: kbae1
Lab 5: Program that uses a social network graph to suggest people that a person
does know and might know.
*/

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

// Write to file for do_know.txt and might_know.txt
void writetofile(string filename, vector<string> vertices,
                 vector<set<int>> vertices_set) {
  int max_length = 0;

  // length of the longest name is determined
  for (int i = 0; i < (int)vertices.size(); i++) {
    max_length = max(max_length, (int)vertices[i].length());
  }

  // write to file stream
  ofstream fout(filename.c_str(), fstream::trunc);

  // iterate through the vertices to print out the output
  for (int i = 0; i < (int)vertices.size(); i++) {
    int counter8 = 1;
    fout << left << setw(max_length) << vertices[i] << " :";
    for (int mysetit : vertices_set[i]) {
      // pretty print with each line only printing out 8 additional names
      if (counter8 % 8 == 1 && counter8 != 1) {
        fout << endl;
        fout << left << setw(max_length) << vertices[i] << " :";
      }
      fout << " " << setw(max_length) << vertices[mysetit];
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
    cerr << "usage: ./Prog5 [-sort] < file.txt" << endl;
    exit(1);
  }
  if (argc == 2 && (string)argv[1] != "-sort") {
    cerr << "usage: ./Prog5 [-sort] < file.txt" << endl;
    exit(1);
  } else if (argc == 2) {
    // parse out if there is a sort argument
    sort = true;
  }

  // read data from stdin create name-to-index map
  string key1, key2;

  // vector to store vertices that represent names read
  vector<string> vertices;

  // vector of sets (BST) stores the indices of adjacent vertices
  vector<pair<int, int>> adjacent_vertices;

  // map to see if name has already been seen
  map<string, int> name_map;

  // vertice pair using index ints for names
  vector<set<int>> edge_list;

  // vector for mapping sorted data - only used if sorting is true
  vector<pair<string, int>> sortedvec = {};

  // read ino cin and insert into name_map
  while (cin >> key1 >> key2) {
    name_map.insert(make_pair(key1, name_map.size()));
    name_map.insert(make_pair(key2, name_map.size()));
    adjacent_vertices.push_back(make_pair(name_map[key1], name_map[key2]));
  }

  if (sort) {
    // Update integer values in (sorted)
    vector<int> changedint(name_map.size());
    int sortedorder = 0;

    // iterate through name_map and populate the changedint
    for (map<string, int>::iterator iter = name_map.begin();
         iter != name_map.end(); ++iter) {
      changedint[iter->second] = sortedorder;
      iter->second = sortedorder;
      sortedorder++;
    }

    // for adj_vertices, change the first and second int
    for (pair<int, int> &myvector : adjacent_vertices) {
      myvector.first = changedint[myvector.first];
      myvector.second = changedint[myvector.second];
    }
  }

  edge_list.resize(name_map.size());

  // insert the adjacent vertices into edge_list
  // keeps track of integers for each name (index)
  for (int k = 0; k < (int)adjacent_vertices.size(); k++) {
    int i = adjacent_vertices[k].first;
    int j = adjacent_vertices[k].second;
    edge_list[i].insert(j);
    edge_list[j].insert(i);
  }

  vertices.resize(name_map.size());

  // populate vertices so each vertice has a string name
  map<string, int>::iterator myit = name_map.begin();
  while (myit != name_map.end()) {
    vertices[myit->second] = myit->first;
    ++myit;
  }

  // write to file
  string fname_out = "do_know.txt";
  writetofile(fname_out, vertices, edge_list);

  // for friends_of_friends
  vector<set<int>> friends_of_friends;
  friends_of_friends.resize(name_map.size());

  int myvertice = 0;
  // check if the potential friend is a listed friend
  for (set<int> setlist : edge_list) {
    for (int myfriend : setlist) {
      for (int potentialfriend : edge_list[myfriend]) {
        if (setlist.count(potentialfriend) == 0 &&
            potentialfriend != myvertice) {
          // if not, insert into the friends_of_friends vector
          friends_of_friends[myvertice].insert(potentialfriend);
        }
      }
    }
    myvertice++;
  }

  // write to might_know
  fname_out = "might_know.txt";
  writetofile(fname_out, vertices, friends_of_friends);

}
