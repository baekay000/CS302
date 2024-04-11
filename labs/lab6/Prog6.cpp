/*
Kaylee Bae
Finds the shortest valid route between any two cities based on geodesic
distances or time. Implements Dijstra's algorithm.
*/

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

#define DEG2RAD (M_PI / 180.0)
#define RAD2DEG (180.0 / M_PI)

const float earthradius = 3963.0;    // [miles]
const float distance_accuracy = 5.0; // [miles]

const int national_minpop = 1000000;

const float national_dist = 150.0; // [miles]
const float regional_dist = 100.0; // [miles]

const float local_maxdist = 50.0; // [miles]

const float plane_speed = 520.0; // [mph]
const float truck_speed = 65.0;  // [mph]

enum city_t { LOCAL, METRO, REGIONAL, NATIONAL };
enum color_t { WHITE, BLACK };

// City labels
// NATIONAL: national_minpop < metro_pop
//           maxpop < national_dist
// REGIONAL: maxpop < regional_dist
// LOCAL:    otherwise

// Edge connectivity (bidirectional)
// NATIONAL: all NATIONAL cities
// REGIONAL: three nearest NON-LOCAL cities
// LOCAL:    five nearest NON-LOCAL cities
//           all LOCAL cities < local_maxdist

class city {
private:
  string city_name;
  city_t type;
  float latitude;
  float longitude;
  int population;

public:
  static int w;

  // Add get variables
  string get_city_name() const { return city_name; };
  city_t get_city_t() const { return type; };
  float get_latitude() const { return latitude; };
  float get_longitude() const { return longitude; };
  int get_population() const { return population; };

  // Add set variables
  void set_city_name(string new_city_name) { city_name = new_city_name; };
  void set_city_t(city_t new_city_t) { type = new_city_t; };
  void set_latitude(float new_lat) { latitude = new_lat; };
  void set_longitude(float new_long) { longitude = new_long; };
  void set_population(int new_pop) { population = new_pop; };

  // Less-than operator
  bool operator<(const city &other) const {
    // Compare city names but put largest first and smallest last
    return population > other.population;
  }
};

// Initalize variable with longest ciy name lengths to 0
int city::w = 0;

class matrix {
private:
  // 1-D vector to store distance and time table information
  vector<float> matrix_vector;

public:
  void resize_vector(int new_size) { matrix_vector.resize(new_size); };

  // get value using math
  // allos for i and j to be in any order
  float get_value(int row, int column) {
    int index;
    if (row > column) {
      index = ((row * (row + 1) / 2) + column);
    } else {
      index = ((column * (column + 1) / 2) + row);
    }

    return matrix_vector[index];
  }

  // set value using the same math
  void set_value(int row, int column, float distance) {
    if (row > column) {
      matrix_vector[(row * (row + 1)) / 2 + column] = distance;
    } else {
      matrix_vector[(column * (column + 1)) / 2 + row] = distance;
    }
  };
};

// takes in city and assigns private variables
istream &operator>>(istream &in, city &mycity) {

  // Set variables
  string city_name;
  string my_type;
  float deg_latitude;
  float deg_longitude;
  int my_population;

  // stores data inside local variables
  in >> city_name >> my_type >> deg_latitude >> deg_longitude >> my_population;

  // assign city name
  mycity.set_city_name(city_name);

  // assign type
  if (my_type == "LOCAL") {
    mycity.set_city_t(LOCAL);
  } else {
    mycity.set_city_t(METRO);
  }

  // assign lat and long using float
  mycity.set_latitude(deg_latitude * DEG2RAD);
  mycity.set_longitude(deg_longitude * DEG2RAD);

  // set population
  mycity.set_population(my_population);

  // check if it is the longest length
  if (city::w < (int)city_name.length()) {
    city::w = (int)city_name.length();
  }
  return in;
};

// outputs the city for writing
ostream &operator<<(ostream &out, city &mycity) {
  string my_type;
  city_t my_city_type = mycity.get_city_t();

  // converts to string
  if (my_city_type == LOCAL) {
    my_type = "LOCAL";
  } else if (my_city_type == METRO) {
    my_type = "METRO";
  } else if (my_city_type == REGIONAL) {
    my_type = "REGIONAL";
  } else if (my_city_type == NATIONAL) {
    my_type = "NATIONAL";
  }

  // assign lat and long using float
  float deg_latitude = mycity.get_latitude() * RAD2DEG;
  float deg_longitude = mycity.get_longitude() * RAD2DEG;

  out << left << setfill('.') << setw(city::w + 3) << mycity.get_city_name()
      << "  " << setfill(' ') << setw(8) << my_type << "  " << right << setw(8)
      << mycity.get_population() << " " << setw(8) << fixed << setprecision(2)
      << deg_latitude << " " << setw(8) << setprecision(2) << deg_longitude
      << endl;

  return out;
};

void create_vertex_table(const char *fname, vector<city> &vertex_table) {
  // read the contents of uscities
  ifstream fin(fname);
  string line;

  while (getline(fin, line)) {

    // remove commas and combine the city and state together
    replace(line.begin(), line.end(), ',', ' ');
    line.replace(line.find(" "), 1, "_");

    city mycity;

    istringstream istream(line);
    istream >> mycity;

    // push back into the vertex_table
    vertex_table.push_back(mycity);
  }

  // sort the vertex_table based on its population
  sort(vertex_table.begin(), vertex_table.end(),
       [](const city &a, const city &b) {
         return a.get_population() > b.get_population();
       });

  fin.close();
}

void update_vertex_table(vector<city> &vertex_table, matrix &dist_table) {
  // implement rule 1 for metro cities
  for (int i = 0; i < (int)vertex_table.size(); i++) {
    city &this_city = vertex_table[i];
    if (this_city.get_city_t() == METRO) {
      // check population to set metro as national or regional
      if (this_city.get_population() > national_minpop) {
        this_city.set_city_t(NATIONAL);
      } else {
        this_city.set_city_t(REGIONAL);
      }
    }
  }

  // implement rule 2 for national cities
  for (int row = 0; row < (int)vertex_table.size(); row++) {
    city &this_city = vertex_table[row];

    if (this_city.get_city_t() == NATIONAL) {
      // iterate through this row to see if it is close to other national cities
      for (int column = 0; column < row; column++) {
        // see if the city at column, row is also NATIONAL
        city &other_city = vertex_table[column];
        if (other_city.get_city_t() == NATIONAL &&
            dist_table.get_value(row, column) < national_dist) {
          this_city.set_city_t(REGIONAL);
          break;
        }
      }
    }
  }

  // implement rule 3 for regional cities
  for (int row = 0; row < (int)vertex_table.size(); row++) {
    city &this_city = vertex_table[row];

    if (this_city.get_city_t() == REGIONAL) {
      // iterate through this row to see if it is close to other regional cities
      for (int column = 0; column < row; column++) {
        // see if the city at column, row is also NATIONAL
        city &other_city = vertex_table[column];

        // if other regional city is close, change to local
        if (other_city.get_city_t() == REGIONAL &&
            dist_table.get_value(row, column) < regional_dist) {
          this_city.set_city_t(LOCAL);
          break;
        }
      }
    }
  }
}

// formula for calculating distance
double haversine_formula(double myangle) { return ((1 - cos(myangle)) / 2); }

void create_dist_table(vector<city> &vertex_table, matrix &dist_table) {
  double haversine_angle = 0.00;

  // // 1D index where index goes from 0 to ((x^2) + 3x) / 2
  int size = ((vertex_table.size() * vertex_table.size()) +
              (3 * vertex_table.size())) /
                 2 +
             1;

  dist_table.resize_vector(size);

  // iterate through rows and columns o create the dist_table
  for (int row = 0; row < (int)vertex_table.size(); row++) {
    for (int column = 0; column <= row; column++) {

      haversine_angle =
          haversine_formula((vertex_table[row]).get_latitude() -
                            (vertex_table[column]).get_latitude()) +
          cos(vertex_table[column].get_latitude()) *
              cos(vertex_table[row].get_latitude()) *
              haversine_formula(vertex_table[row].get_longitude() -
                                vertex_table[column].get_longitude());

      // calculate distance and round to the nearest 5.0 mile
      float final_distance =
          5.0 * round(2.0 * earthradius * asin(sqrt(haversine_angle)) / 5.0);

      dist_table.set_value(row, column, final_distance);
    }
  }
}
void create_time_table(vector<city> &vertex_table, matrix &dist_table,
                       matrix &time_table) {
  city this_city;
  city_t this_city_t;
  city other_city;

  bool national;
  int size = ((vertex_table.size() * vertex_table.size()) +
              (3 * vertex_table.size())) /
                 2 +
             1;
  time_table.resize_vector(size);

  // create time table by iterating through the vertex table
  for (int row = 0; row < (int)vertex_table.size(); row++) {
    this_city = vertex_table[row];
    this_city_t = this_city.get_city_t();

    // check if this city is national
    if (this_city_t == NATIONAL) {
      national = true;
    } else {
      national = false;
    }

    // if national, calculate time between using the airplane travel times
    for (int column = 0; column <= row; column++) {
      float distance = dist_table.get_value(row, column);
      other_city = vertex_table[column];

      // divides the distance by the travel method
      if (national && other_city.get_city_t() == NATIONAL) {
        time_table.set_value(row, column, distance / plane_speed);
      } else {
        // use truck_speed
        time_table.set_value(row, column, distance / truck_speed);
      }
    }
  }
}

void create_edge_table(vector<city> &vertex_table, vector<set<int>> &edge_table,
                       matrix &dist_table) {

  edge_table.resize(vertex_table.size());

  // iterate through the row and implement the rules based on city_t
  for (int row = 0; row < (int)vertex_table.size(); row++) {

    // if national, edges are connected to one another but not itself
    if (vertex_table[row].get_city_t() == NATIONAL) {
      for (int column = 0; column < (int)vertex_table.size(); column++) {
        if (vertex_table[column].get_city_t() == NATIONAL && column != row) {
          edge_table[row].insert(column);
        }
      }

      // if regional, connect to the three closest nonlocal cities
    } else if (vertex_table[row].get_city_t() == REGIONAL) {
      vector<int> temp_vector = {};

      // push back into a temporary vector if nonlocal
      for (int column = 0; column < (int)vertex_table.size(); column++) {
        if (vertex_table[column].get_city_t() != LOCAL && column != row) {
          temp_vector.push_back(column);
        }
      }

      // partially sort the vector using partial_sort function
      partial_sort(temp_vector.begin(), temp_vector.begin() + 3,
                   temp_vector.end(),
                   [&dist_table, row](const int &first, const int &second) {
                     return dist_table.get_value(row, first) <
                            dist_table.get_value(row, second);
                   });

      // push back only the first 3 into the edge table
      for (int i = 0; i < 3; i++) {
        edge_table[row].insert(temp_vector[i]);
        edge_table[temp_vector[i]].insert(row);
      }

    } else {
      // for local cities
      vector<int> temp_vector = {};

      for (int column = 0; column < (int)vertex_table.size(); column++) {
        if (column == row) {
          continue;
        }

        // if local cities are close to one another, push back
        if (vertex_table[column].get_city_t() == LOCAL) {
          if (dist_table.get_value(row, column) < local_maxdist) {
            edge_table[row].insert(column);
            edge_table[column].insert(row);
          }

        } else {
          // use temp vector to store cities
          temp_vector.push_back(column);
        }
      }

      // find the 5 closest cities
      partial_sort(temp_vector.begin(), temp_vector.begin() + 5,
                   temp_vector.end(),
                   [&dist_table, row](const int &first, const int &second) {
                     return dist_table.get_value(row, first) <
                            dist_table.get_value(row, second);
                   });

      // push back closest 5
      for (int i = 0; i < 5; i++) {
        edge_table[row].insert(temp_vector[i]);
        edge_table[temp_vector[i]].insert(row);
      }
    }
  }
}

void write_vertex_table(vector<city> &vertex_table) {
  // write the processed city information back to the file
  ofstream fout("vertex_table.txt");
  int i = 0;

  // iterate through vertex_table to write out the number and city information
  for (city mycity : vertex_table) {
    fout << setw(4) << i << "  " << mycity;
    i++;
  }
  fout.close();
}

void write_dist_table(vector<city> &vertex_table, matrix &dist_table) {
  ofstream fout("dist_table.txt");
  int row = 0;
  int width = city::w + 3;

  // iterate through vertex_table
  for (city mycity : vertex_table) {
    if (row == 0) {
      row++;
      continue;
    }

    // if the column is less than the row, print out the distance table
    for (int column = 0; column < row; column++) {

      fout << right << setw(4) << row << "  " << left << setw(width)
           << setfill('.') << mycity.get_city_name() << " to " << setw(width)
           << vertex_table[column].get_city_name() << "  " << setw(6)
           << setfill(' ') << fixed << setprecision(1) << right
           << dist_table.get_value(row, column) << " miles" << endl;
    }
    row++;
    fout << endl;
  }
  fout.close();
}

void write_time_table(vector<city> &vertex_table, matrix &time_table) {
  ofstream fout("time_table.txt");
  int row = 0;

  // same thing but with time
  for (city mycity : vertex_table) {
    if (row == 0) {
      row++;
      continue;
    }

    for (int column = 0; column < row; column++) {
      // use the time_table instead of the distance_table
      fout << right << setw(4) << row << "  " << left << setfill('.')
           << setw(city::w + 3) << mycity.get_city_name() << " to "
           << setw(city::w + 3) << vertex_table[column].get_city_name() << "  "
           << setfill(' ') << fixed << setprecision(1) << setw(6) << right
           << time_table.get_value(row, column) << " hours" << endl;
    }
    row++;
    fout << endl;
  }
  fout.close();
}

void write_edge_table(vector<city> &vertex_table, vector<set<int>> &edge_table,
                      matrix &dist_table, matrix &time_table) {
  ofstream fout("edge_table.txt");

  // iterate through the vertex_table
  for (int row = 0; row < (int)vertex_table.size(); row++) {

    fout << right << setw(4) << row << " " << vertex_table[row].get_city_name()
         << endl;

    // for the edge_table in each row, print out the miles and the hours it
    // takes from one city to the other
    for (set<int>::iterator myit = edge_table[row].begin();
         myit != edge_table[row].end(); myit++) {
      fout << right << setw(6) << *myit << " " << left << setfill('.')
           << setw(city::w + 3) << vertex_table[*myit].get_city_name() << "  "
           << right << setfill(' ') << fixed << setprecision(1) << setw(6)
           << dist_table.get_value(row, *myit) << " miles " << setprecision(1)
           << setw(4) << time_table.get_value(row, *myit) << " hours" << endl;
    }

    // print out a new line
    if (row != int(vertex_table.size() - 1)) {
      fout << endl;
    }
  }
  fout.close();
}

void dijkstra_route(int city_from, int city_to, vector<city> &vertex_table,
                    vector<set<int>> &edge_table, bool mode, matrix &dist_table,
                    matrix &time_table) {

  // Code copied from Dr. Gregor's handout

  vector<color_t> vcolor;
  vector<float> vdist;
  vector<int> vlink;

  // same variables used
  vcolor.assign(vertex_table.size(), WHITE);
  vdist.assign(vertex_table.size(), numeric_limits<float>::max());
  vlink.assign(vertex_table.size(), -1);
  vlink[city_from] = city_from;
  vdist[city_from] = 0;

  while (1) {
    int next_i = -1;
    float mindist = numeric_limits<float>::max();

    for (int i = 0; i < (int)vcolor.size(); i++) {
      if (vcolor[i] == WHITE && mindist > vdist[i]) {
        next_i = i;
        mindist = vdist[i];
      }
    }

    int i = next_i;
    if (i == -1) {
      return;
    }
    vcolor[i] = BLACK;
    if (i == city_to) {
      break;
    }

    // changed this to a set iterator instead of a for loop
    for (set<int>::iterator myit = edge_table[i].begin();
         myit != edge_table[i].end(); myit++) {

      int j = *myit;

      float wij = 0;

      // if mode, then that means that the user used -dist
      // else, -time
      if (mode) {
        wij = dist_table.get_value(i, j);
      } else {
        // mode affects the weighting
        wij = time_table.get_value(i, j);
      }

      if (vcolor[j] == BLACK) {
        continue;
      }

      if (vdist[j] > vdist[i] + wij) {
        vdist[j] = vdist[i] + wij;
        vlink[j] = i;
      }
    }
  }

  // push all the integer values into a stack
  stack<int> S;
  for (int i = city_to; i != city_from; i = vlink[i]) {
    S.push(i);
  }

  int j = city_from;
  float dist_total = 0;
  float time_total = 0;

  // print out city_in city
  cout << setw(8) << fixed << setprecision(2) << time_total << " miles"
       << setw(6) << dist_total << " hours" << setw(5) << city_from << " "
       << left << setfill('.') << setw(city::w + 3)
       << vertex_table[city_from].get_city_name() << right << setfill(' ')
       << setw(10) << vertex_table[city_from].get_population() << endl;

  // iterate through the stack and print out city information of that city
  while (!S.empty()) {
    int i = S.top();

    // keep a running total of the total time and distance travelled
    dist_total += dist_table.get_value(i, j);
    time_total += time_table.get_value(i, j);

    S.pop();

    // match gradscript
    cout << right << setw(8) << dist_total << " miles" << setw(6) << time_total
         << " hours" << setw(5) << i << " " << left << setfill('.')
         << setw(city::w + 3) << vertex_table[i].get_city_name() << "  "
         << setw(8) << setfill(' ') << right << vertex_table[i].get_population()
         << " " << setw(7) << dist_table.get_value(i, j) << " miles  "
         << setw(4) << time_table.get_value(i, j) << " hours" << endl;
    j = i;
  }
}

int main(int argc, char *argv[]) {

  bool info = false;
  bool dist = false;
  bool time = false;
  vector<city> vertex_table;
  vector<set<int>> edge_table;

  // parse commandline arguments for info, dist, and time
  if (argc != 3 && argc != 4) {
    cerr << "usage: ./Prog6 -info|dist|time [-seed=N] cities.txt" << endl;
    return 1;
  }
  
  if (string(argv[1]) == "-info") {
    info = true;
  } else if (string(argv[1]) == "-dist") {
    dist = true;
  } else if (string(argv[1]) == "-time") {
    time = true;
  } else {
    cerr << "usage: ./Prog6 -info|dist|time [-seed=N] cities.txt" << endl;
    return 1;
  }

  // if (argc == 4) {
  //   // written using Dr. Gregor's example code on Canvas
  //   string new_label = strtok(argv[2], "=");
  //   seedvalue = atoi(strtok(NULL, "="));
  // }

  // grab filename
  string filename = argv[argc - 1];
  const char *fname = filename.c_str();

  // create vertex_table
  create_vertex_table(fname, vertex_table);

  // create dist_table update vertex_table create time_table
  matrix dist_table;
  matrix time_table;
  create_dist_table(vertex_table, dist_table);
  update_vertex_table(vertex_table, dist_table);
  create_time_table(vertex_table, dist_table, time_table);
  create_edge_table(vertex_table, edge_table, dist_table);

  // write to these files if -info
  if (info) {
    write_vertex_table(vertex_table);
    write_dist_table(vertex_table, dist_table);
    write_time_table(vertex_table, time_table);
    write_edge_table(vertex_table, edge_table, dist_table, time_table);
  } else {
    
    // this means -dist or -time
    string city1;
    string city2;
    cout << "Enter> ";

    int city_from;
    int city_to;

    // store cin user input into variables
    while (cin >> city1 >> city2) {
      city_from = -1;
      city_to = -1;

      // check if city_to and city_from are in the list of known cities
      for (int i = 0; i < (int)vertex_table.size(); i++) {
        if (vertex_table[i].get_city_name() == city1) {
          city_from = i;
        }
        if (vertex_table[i].get_city_name() == city2) {
          city_to = i;
        }
      }

      // if not, print out the error message
      if (city_from < 0 || city_to < 0) {
        cout << "Invalid_city: prefix not found" << endl << endl;
        // set city_from equal to 0
        cout << "Enter> ";
        continue;
      }

      // mode true means that it is distance, false == time
      if (dist) {
        dijkstra_route(city_from, city_to, vertex_table, edge_table, true,
                       dist_table, time_table);
      } else {
        dijkstra_route(city_from, city_to, vertex_table, edge_table, false,
                       dist_table, time_table);
      }

      cout << endl;

      cout << "Enter> ";
    }
    cout << endl;
  }
}
