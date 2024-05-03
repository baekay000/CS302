#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// CPP,php,python,js,java,.net

using namespace std;

#include "Support.h"

int main(int argc, char *argv[]) {
  // declare variables
  person *n_person;
  student *n_student;
  faculty *n_faculty;

  vector<person *> person_list;
  vector<string> course_list;
  vector<float> gp_list;

  // 3 settings: person, faculty, student
  bool person_mode = false;
  bool faculty_mode = false;
  bool student_mode = false;

  // process commandline arguments
  if (argc != 3) {
    cerr << "usage: ./sprog8 -person|faculty|student file.xml" << endl;
    return 1;
  }

  if ((string)argv[1] == "-person") {
    person_mode = true;
  } else if ((string)argv[1] == "-faculty") {
    faculty_mode = true;
  } else if ((string)argv[1] == "-student") {
    student_mode = true;
  } else {
    cerr << "usage: ./sprog8 -person|faculty|student file.xml" << endl;
    return 1;
  }

  // handle commandline options
  string fname = argv[argc - 1];

  label type = UNKNOWN;
  label category = UNKNOWN;

  int line = 1;
  string input;

  // open input file stream fin
  ifstream fin(fname);

  // error check: file open?
  if (!fin.is_open()) {
    cerr << "zsh: no such file or directory: " << fname << endl;
    return 1;
  }

  // read XML title line
  const string facultytag = "faculty";
  const string studenttag = "student";
  const string nametag = "name";

  // 2 types: faculty, student
  bool is_faculty = false;
  bool is_student = false;

  // save current category
  string my_name;

  while (getline(fin, input)) {
    ++line;

    // skip first line
    if (line == 2) {
      continue;
    }

    // input does not contain < character
    ulong first_tag = input.find('<');
    if (first_tag == string::npos) {
      continue;
    }

    // input is <faculty> tag
    first_tag = input.find('<' + facultytag);
    if (first_tag != string::npos) {
      is_faculty = true;
      course_list.clear();
      continue;
    }

    // input is </faculty> tag
    first_tag = input.find('/' + facultytag);
    if (first_tag != string::npos) {
      if (!is_faculty) {
        cout << "incorrect" << endl;
      } // file is in incorrect format

      // create n_person faculty object
      faculty *n_faculty = new faculty(my_name, type, category);

      // sort courses

      // add courses
      for (string course : course_list) {
        n_faculty->add_course(course);
      }

      // add n_person to person_list
      person_list.push_back(n_faculty);

      // reset type, category, faculty
      type = UNKNOWN;
      category = UNKNOWN;
      is_faculty = false;

      continue;
    }

    // input is <student> tag
    first_tag = input.find('<' + studenttag);
    if (first_tag != string::npos) {
      is_student = true;

      // clear course_list clear gp_list
      course_list.clear();
      gp_list.clear();

      continue;
    }

    // input is </student> tag
    first_tag = input.find('/' + studenttag);
    if (first_tag != string::npos) {
      if (!is_student) {
        cout << "incorrect" << endl;
      } // file is in incorrect format

      // create n_person student object
      student *n_student = new student(my_name, type, category);

      // TODO: add course, gp pairs from course_list, gp_list

      // add n_person to person_list
      person_list.push_back(n_student);

      // reset type, category, faculty
      type = UNKNOWN;
      category = UNKNOWN;
      is_student = false;

      continue;
    }

    // input is <name> tag
    ulong start = input.find(nametag + '=');
    if (start != string::npos) {

      // error check if there is an ending / tag
      ulong end = input.find('/');
      if (end == string::npos) {
        cout << "error" << endl;
      }
      my_name = input.substr(start + 6, input.length() - 12);
    }

    // // parse the next row by creating a substring
    // string person_role = input.substr(first_tag + 1, input.length() - 2);
    // cout << person_role << endl;

    // else if (first_tag + 1 == "student"){
    //   // checks if student or faculty tag

    // }

    // find person -- version 1

    // else if (input.find('<' + facultytag) != string::npos) {
    //   cout << "YAY, FACULTY!!!!" << endl;
    //   string str2 = input.substr(start + 1, start + 8);
    //   cout << str2 << endl;
  }

  // Sort the vector using a lambda function with <
  sort(person_list.begin(), person_list.end(),
       [](person *a, person *b) { return *a < *b; });

  for (person *my_per : person_list) {
    cout << my_per->read_name() << endl;
  }

  // else if (input is < category tag) == 0) {
  //     find and extract category
  //   error check: end-of-tag>

  //   }
  //   else if (input is < course tag) == 0) {
  //       find and extract course add course to course_list

  //               if type STUDENT{find and extract gp add gp to gp_list}

  //               error check : end -
  //                             of - tag >
  //     }
  // }

  // if mode
  //   PERSON {
  //     create list of person object pointers apply std::sort using lambda
  //     less
  //     -
  //         than function write result to stdout
  //   }
  // else if mode
  //   FACULTY {
  //     create list of faculty object pointers apply std::sort using lambda
  //     less -
  //         than function write result to stdout
  //   }
  // else if mode
  //   STUDENT{create list of student object pointers apply
  //               std::sort using lambda less -
  //           than function write result to stdout}

  //   release dynamically allocated memory
}
