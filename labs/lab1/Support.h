#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <list>
#include <string>
#include <vector>
#include <map> 
#include <unordered_map>
#include <set>
using namespace std;

struct location {
  string city;
  string state;
  string geocode;

  bool operator<(const location &) const;
  void print() const;
};

struct rawdata {
  int month;
  float precip;
  int temp;

  void print();
};

struct summary {
  int N;

  float precip_avg;
  float precip_max;
  float precip_min;

  float temp_avg;
  float temp_max;
  float temp_min;

  summary();
  void operator+=(rawdata);
  void print();
};

class database {
  public:
    void init_rawdata(string filename);
    void print_rawdata();

    void init_summary();
    void print_summary(string target);

  private:
    void extract_rawdata(string &line); 
    void extract_summary();

    map <location, int> location_map;
    vector< list <rawdata> > drcache; 
    list<rawdata> drlist;

    unordered_map < string, set<string> > state_map;
    unordered_map < string, location > geocode_map;

    vector< vector <summary> > dscache;

};

#endif