/*
Kaylee Bae
NetID: kbae1
Parses an XML file with student and faculty information to practice the concept
of inheritance.
*/

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
  student *n_student;
  faculty *n_faculty;

  // keeps track of people, faculty course_list, and student course_gp_list
  vector<person *> person_list;
  vector<string> course_list;
  vector<pair<string, float>> course_gp_list;

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

  int line = 0;
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
  const string categorytag = "category";
  const string coursetag = "course";
  const string xmltag = "<?XML";

  // 2 types: faculty, student
  bool is_faculty = false;
  bool is_student = false;

  // save current category
  string my_name;

  while (getline(fin, input)) {
    ++line;

    // skip first line
    if (line == 1) {
      // error check that there is an XML tag
      if (input.find(xmltag) == string::npos) {
        cerr << "line " << line << ": XML declaration missing" << endl;
        return 1;
      }
      continue;
    }

    // input does not contain < character
    if (input.find('<') == string::npos) {
      continue;
    }

    // input is <faculty> tag
    if (input.find('<' + facultytag) != string::npos) {
      // updatetype, is_faculty
      is_faculty = true;
      type = FACULTY;

      // clear relevant vectors
      course_list.clear();
      course_gp_list.clear();
      continue;
    }

    // input is </faculty> tag
    if (input.find('/' + facultytag) != string::npos) {
      if (!is_faculty) {
        cerr << "line " << line << ": expected </student> tag" << endl;
        return 1;
      } // file is in incorrect format

      // create n_person faculty object
      faculty *n_faculty = new faculty(my_name, type, category);

      // sort courses
      sort(course_list.begin(), course_list.end());

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
    if (input.find('<' + studenttag) != string::npos) {
      is_student = true;
      type = STUDENT;

      // clear course_list clear gp_list
      course_list.clear();
      course_gp_list.clear();

      continue;
    }

    // input is </student> tag
    if (input.find('/' + studenttag) != string::npos) {
      if (!is_student) {
        cerr << "line " << line << ": expected </faculty> tag" << endl;
        return 1;
      } // file is in incorrect format

      // create n_person student object
      student *n_student = new student(my_name, type, category);

      // sort the vector using the course names
      sort(course_gp_list.begin(), course_gp_list.end(),
           [](pair<string, float> a, pair<string, float> b) {
             return a.first < b.first;
           });

      // add the courses after sorting
      for (pair<string, float> course : course_gp_list) {
        n_student->add_course(course);
      }

      // add n_person to person_list
      person_list.push_back(n_student);

      // reset type, category, faculty
      type = UNKNOWN;
      category = UNKNOWN;
      is_student = false;

      continue;
    }

    // input is <name> tag
    if (input.find(nametag + '=') != string::npos) {

      // error check if there is an ending / tag
      if (input.find('/') == string::npos) {
        cerr << "line " << line << ": name missing/not terminated" << endl;
        return 1;
      }
      my_name = input.substr(9, input.length() - 12);

      if (is_faculty) {
        // add doctor title
        my_name = "Dr. " + my_name;
      }
      continue;
    }

    // input is <category>
    if (input.find(categorytag + '=') != string::npos) {

      if (input.find('/') == string::npos) {
        cerr << "line " << line << ": category missing/not terminated" << endl;
        return 1;
      }

      // find and extract category
      string category_name = input.substr(13, input.length() - 16);

      // convert string to enum label
      if (category_name == "Assistant Professor") {
        category = ASST_PROF;
      } else if (category_name == "Associate Professor") {
        category = ASSOC_PROF;
      } else if (category_name == "Professor") {
        category = PROFESSOR;
      } else if (category_name == "Freshman") {
        category = FRESHMAN;
      } else if (category_name == "Sophomore") {
        category = SOPHOMORE;
      } else if (category_name == "Junior") {
        category = JUNIOR;
      } else if (category_name == "Senior") {
        category = SENIOR;
      }

      continue;
    }

    // input is <course>
    if (input.find(coursetag + '=') != string::npos) {
      // find and extract course add course to course_list
      if (is_student) {
        // parse out course and gpa information
        string course = input.substr(11, input.length() - 23);
        float gpa = stof(input.substr(input.length() - 6, 3));

        // push back into the correct vector
        course_gp_list.push_back(make_pair(course, gpa));

      } else {
        // faculty do not keep track of GPA
        string course = input.substr(11, input.length() - 14);
        course_list.push_back(course);
      }

      if (input.find('/') == string::npos) {
        cerr << "line " << line << ": category missing/not terminated" << endl;
        return 1;
      }
    }
  }

  if (person_mode) {
    // Sort the vector using a lambda function with <
    sort(person_list.begin(), person_list.end(),
         [](person *a, person *b) { return *a < *b; });

    // print out the person information
    for (int i = 0; i < (int)person_list.size(); i++) {
      cout << *(person_list[i]);
      cout << endl;
    }
  } else if (faculty_mode) {

    // put only the faculty in the faculty vector
    vector<faculty *> faculty_list;
    for (int i = 0; i < (int)person_list.size(); i++) {
      if (person_list[i]->get_type() == FACULTY) {
        faculty_list.push_back(dynamic_cast<faculty *>(person_list[i]));
      }
    }

    // sort the faculty vector
    sort(faculty_list.begin(), faculty_list.end(),
         [](faculty *a, faculty *b) { return *a < *b; });

    // print out information
    for (int i = 0; i < (int)faculty_list.size(); i++) {
      cout << *(faculty_list[i]);
      cout << endl;
    }

  } else {
    // student mode
    vector<student *> student_list;

    // put only student in the vector
    for (int i = 0; i < (int)person_list.size(); i++) {
      if (person_list[i]->get_type() == STUDENT) {
        student_list.push_back(dynamic_cast<student *>(person_list[i]));
      }
    }

    // sort the faculty vector
    sort(student_list.begin(), student_list.end(),
         [](student *a, student *b) { return *a < *b; });

    // print
    for (int i = 0; i < (int)student_list.size(); i++) {
      cout << *(student_list[i]);
      cout << endl;
    }
  }

  // delete allocated memory
  for (person *my_person : person_list) {
    if (my_person->get_type() == FACULTY) {
      delete dynamic_cast<faculty *>(my_person);
    } else {
      delete dynamic_cast<student *>(my_person);
    }
  }
}
