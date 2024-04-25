#include <...>
using namespace std;

#include "Support.h"

int main(int argc, char *argv[]) {
  handle commandline options

  person *n_person;
  vector<person *> person_list;
  vector<string> course_list;
  vector<float> gp_list;

  label type = UNKNOWN;
  label category = UNKNOWN;

  int line = 1;
  string input;

  open input file stream fin
  error check: file open?

  read XML title line

  while (getline(fin, input)) {
    ++line;

    if (input does not contain < character) {
	  continue;

    } else if (input is <faculty> tag) {
      set type FACULTY

      clear course_list

    } else if (input is </faculty> tag) {
      error check: type FACULTY?

      set category

      create n_person FACULTY object 
	  add courses from course_list

      add n_person to person_list

      reset type
      reset category

    } else if (input is <student> tag) {
      set type STUDENT

      clear course_list
      clear gp_list

    } else if (input is </student> tag) {
      error check: type STUDENT?

      set category

      create n_person STUDENT object 
	  add course, gp pairs from course_list, gp_list

      add n_person to person_list

      reset type
      reset subtype

    } else if (input is <name tag) {
      find and extract name
	  error check: end-of-tag>

    } else if (input is <category tag) == 0) {
      find and extract category
	  error check: end-of-tag>

    } else if (input is <course tag) == 0) {
      find and extract course
	  add course to course_list

	  if type STUDENT {
	    find and extract gp
	    add gp to gp_list
      }

	  error check: end-of-tag>
    }
  }

  if mode PERSON {
    create list of person object pointers
	apply std::sort using lambda less-than function
	write result to stdout
  } else
  if mode FACULTY {
    create list of faculty object pointers
	apply std::sort using lambda less-than function
	write result to stdout
  } else
  if mode STUDENT {
    create list of student object pointers
	apply std::sort using lambda less-than function
	write result to stdout
  } 

  release dynamically allocated memory
}
