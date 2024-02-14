/*
Kaylee Bae
Implements constructor and functions for structs 'rawdata, location, summary'
and class 'database'.
*/

#include "Support.h"
#include <algorithm>
#include <cfloat>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;

// Ensures city summaries are sorted first by state and then by city
bool location::operator<(const location &other) const {
  if (state == other.state) {
    if (city == other.city) {
      return geocode < other.geocode;
    }
    return city < other.city;
  } else {
    return state < other.state;
  }
}

// output the location information with each line being 42 characters wide
void location::print() const {
  // Dashes
  for (int i = 0; i < 42; i++) {
    cout << "-";
  }
  cout << endl;

  // Print output
  cout << left << city << ", " << state << " (" << geocode << ")" << endl;

  // Dashes
  for (int i = 0; i < 42; i++) {
    cout << "-";
  }
  cout << endl;
}

// Prints out rawdata
void rawdata::print() {
  cout << right << setw(5) << month << fixed << setprecision(2) << setw(6)
       << precip << setw(6) << temp << endl;
}

// Stores data inside location and data variables
void database::init_rawdata(string filename) {
  location n_location;
  rawdata n_rawdata;
  string line;

  ifstream fin(filename);

  // ifstream error
  if (!fin.is_open()) {
    cerr << "error: cannot open invalid_file.csv" << endl;
    cerr << "usage: ./Prog1 [-rawdata] datafile" << endl;
    exit(1);
  }

  while (getline(fin, line)) {

    // calls database member function that removes dashes
    extract_rawdata(line);

    istringstream ss(line);

    // stores data inside local variables
    ss >> n_rawdata.month >> n_location.city >> n_location.state >>
        n_location.geocode >> n_rawdata.precip >> n_rawdata.temp;

    int index = drcache.size();

    // insert data
    pair<map<location, int>::iterator, bool> insertionResult =
        location_map.insert({n_location, index});

    if (insertionResult.second) {
      // was inserted, element DNE
      list<rawdata> newlist = {n_rawdata};
      drcache.push_back(newlist);

    } else {
      // element existed, push back into existing list
      drcache[insertionResult.first->second].push_back(n_rawdata);
    }
  }
}

// vector of months for summary data
vector<string> months = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// populate state_map and geocode_map hashtables
void database::init_summary() {
  map<location, int>::iterator p = location_map.begin();

  while (p != location_map.end()) {
    // add geocode to set
    set<string> myset = {p->first.geocode};

    // try to insert
    pair<unordered_map<string, set<string>>::iterator, bool> insertionResult =
        state_map.insert({p->first.state, myset});

    // state exists in state_map
    if (!insertionResult.second) {
      // insert geocode inside existing state
      state_map[p->first.state].insert(p->first.geocode);
    }

    // if there is not a geocode already
    if (!geocode_map.count(p->first.geocode)) {
      // insert into geocode_map
      geocode_map.insert({p->first.geocode, p->first});
    }
    p++;
  }
  // call extract to populate monthly summaries
  extract_summary();
}

// populate monthly summaries in dscache
void database::extract_summary() {
  map<location, int>::iterator mapit = location_map.begin();

  // resize dscache to align with location_map
  dscache.resize(location_map.size());

  // go through location_map to ensure that indices align
  while (mapit != location_map.end()) {
    int index = mapit->second;

    // resize to 12, one for each month
    dscache[index].resize(12);

    list<rawdata>::iterator rawdatait = drcache[index].begin();

    // iterate through drcache to populate dscache
    while (rawdatait != drcache[index].end()) {
      int monthindex = (rawdatait->month) - 1;

      // use overloaded operator to add rawdata
      dscache[index][monthindex] += *rawdatait;

      rawdatait++;
    }

    // update average temperature and precipitation
    for (int i = 0; i < (int)dscache[index].size(); i++) {
      dscache[index][i].temp_avg =
          dscache[index][i].temp_avg / dscache[index][i].N;
      dscache[index][i].precip_avg =
          dscache[index][i].precip_avg / dscache[index][i].N;
    }

    mapit++;
  }
}

// print out all rawdata from the file
void database::print_rawdata() {

  map<location, int>::iterator p = location_map.begin();
  list<rawdata>::iterator listit;

  // iterate through location_map
  while (p != location_map.end()) {
    // Print location
    (p->first).print();

    listit = drcache[p->second].begin();

    // print out the raw data in the list
    while (listit != drcache[p->second].end()) {
      listit->print();
      listit++;
    }

    p++;
  }
};

// replace spaces with underscores and commas with spaces
void database::extract_rawdata(string &line) {

  replace(line.begin(), line.end(), ' ', '_');
  replace(line.begin(), line.end(), ',', ' ');
}

// constructor to initialize class data members
summary::summary() {
  N = 0;

  precip_avg = 0;
  precip_max = FLT_MIN;
  precip_min = FLT_MAX;

  temp_avg = 0;
  temp_max = FLT_MIN;
  temp_min = FLT_MAX;
}

// updating summaries with the raw data
void summary::operator+=(rawdata n_rawdata) {
  // updating the number of records seen
  N++;

  // Updating precipitation with new data
  precip_avg += n_rawdata.precip;
  if (n_rawdata.precip > precip_max) {
    precip_max = n_rawdata.precip;
  }
  if (n_rawdata.precip < precip_min) {
    precip_min = n_rawdata.precip;
  }

  // Updating temperatures with new data
  temp_avg += n_rawdata.temp;
  if (n_rawdata.temp > temp_max) {
    temp_max = n_rawdata.temp;
  }
  if (n_rawdata.temp < temp_min) {
    temp_min = n_rawdata.temp;
  }
}

// Prints output with correct format
void summary::print() {
  cout << fixed << right << setprecision(2) << setw(6) << precip_min << setw(6)
       << precip_max << setw(6) << precip_avg << " : " << setprecision(1)
       << setw(5) << temp_min << setw(6) << temp_max << setw(6) << temp_avg
       << endl;
  cout.flush();
}

void database::print_summary(string target) {
  set<string> geocode_set;
  queue<string> geocode_queue;

  // if target was in state map
  if (state_map.find(target) != state_map.end()) {

    // iterate through geocode_set
    geocode_set = state_map.at(target);
    set<string>::iterator setit = geocode_set.begin();

    while (setit != geocode_set.end()) {
      // add to geocode_queue
      geocode_queue.push(*setit);
      setit++;
    }

  } else if (geocode_map.find(target) != geocode_map.end()) {
    // if geocode is found in geocode_map
    geocode_queue.push(target);

  } else {
    cerr << "Target not found!" << endl;
    exit(1);
  }

  // go through all geocodes in queue
  while (geocode_queue.size() != 0) {

    location mylocation = geocode_map[geocode_queue.front()];
    int index = location_map[mylocation];

    // print out location
    mylocation.print();

    // print out vector of summaries
    for (int i = 0; i < (int)dscache[index].size(); i++) {
      cout << months[i] << ":";
      dscache[index][i].print();
    }

    // pop off first element to print out other geocodes
    geocode_queue.pop();
  }
}