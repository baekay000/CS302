#include ...
using namespace std;

#define DEG2RAD (M_PI/180.0)
#define RAD2DEG (180.0/M_PI)

const float earthradius = 3963.0;     // [miles]
const float distance_accuracy = 5.0;  // [miles]

const int national_minpop = 1000000;

const float national_dist = 150.0;    // [miles]
const float regional_dist = 100.0;    // [miles]

const float local_maxdist = 50.0;     // [miles]

const float plane_speed = 520.0;      // [mph]
const float truck_speed = 65.0;       // [mph]

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

class city { ... }
class matrix { ... }

istream & operator>>(istream &in, city &c) { ... }
ostream & operator<<(ostream &out, const city &c) { ... }

void create_vertex_table(char *fname, datatype &vertex_table) { ... }
void update_vertex_table(datatype &vertex_table, matrix &dist_table) { ... }

void create_dist_table(datatype &vertex_table, matrix &dist_table) { ... }
void create_time_table(datatype &vertex_table, matrix &dist_table, matrix &time_table) { ... }

void create_edge_table(datatype &vertex_table, datatype &edge_table, matrix &dist_table) { ... }

void write_vertex_table(datatype &vertex_table) { ... }
void write_dist_table(datatype &vertex_table, matrix &dist_table) { ... }
void write_time_table(datatype &vertex_table, matrix &time_table) { ... }
void write_edge_table(datatype &vertex_table, datatype &edge_table,
                      matrix &dist_table, matrix &time_table) { ... }

void dijkstra_route(int city_from, int city_to, datatype &vertex_table, datatype &edge_table,
                    datatype mode, matrix &dist_table, matrix &time_table) { ... }

int main(int argc, char *argv[]) {
  parse commandline arguments

  create vertex_table

  create dist_table
  update vertex_table
  create time_table

  create edge_table

  if INFO mode,
    write tables to file
	exit

  while (not done) {
    prompt user for city_from, city_to
    dijkstra_route(city_from, city_to, DIST or TIME mode, ...)
  }
}
