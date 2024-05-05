/*
Kaylee Bae
NetID: kbae1
Implementations of the functions in Support.h
*/

#include <iostream>
using namespace std;

#include "Support.h"
#include <iomanip>

// person class member function implemention
person::person(){
  ;
}

    // operator function prints out the person's info and courses (latter calls
    // faculty/student * functions)
    void
    operator<<(ostream &out, person &other) {
  other.print_personinfo();
  other.print_courseinfo();
};

// compares people, sorted first by type then by name
bool person::operator<(person &b) {
  if (type != b.type) {
    return type < b.type;
  } else {
    return name < b.name;
  }
}

// returns protected type
label person::get_type() { return type; };

// prints out person information
void person::print_personinfo() {
  cout << right << setw(12) << "Name: " << name << endl;

  // changes enum to string
  string category_s;
  if (category == ASST_PROF) {
    category_s = "Assistant Professor";
  } else if (category == ASSOC_PROF) {
    category_s = "Associate Professor";
  } else if (category == PROFESSOR) {
    category_s = "Professor";
  } else if (category == FRESHMAN) {
    category_s = "Freshman";
  } else if (category == SOPHOMORE) {
    category_s = "Sophomore";
  } else if (category == JUNIOR) {
    category_s = "Junior";
  } else if (category == SENIOR) {
    category_s = "Senior";
  } else {
    cout << "ERROR" << endl;
  }

  // prints out category
  cout << right << setw(12) << "Category: " << category_s << endl;
};

void person::print_courseinfo() {
  // base function
  ;
}

// faculty class member function implemention

faculty::faculty(string new_name, label new_type, label new_category) {
  name = new_name;
  type = new_type;
  category = new_category;
}

// push back new courses into the vector
void faculty::add_course(string new_course) {
  courselist.push_back(new_course);
}

// compare faculty by category, then name
bool faculty::operator<(faculty &b) {
  if (category != b.category) {
    return category > b.category;
  } else {
    return name < b.name;
  }
}

void faculty::print_courseinfo() {
  // iterate through courses and print out courses
  for (string course : courselist) {
    cout << right << setw(12) << "Course: " << course << endl;
  }
}

// student class member function implemention

// constructor
student::student(string new_name, label new_type, label new_category) {
  name = new_name;
  type = new_type;
  category = new_category;
}

// add to vector
void student::add_course(pair<string, float> mypair) {
  courselist.push_back(mypair);
}

// compare by category
bool student::operator<(student &b) {
  if (category != b.category) {
    return category < b.category;
  } else {
    return name < b.name;
  }
}

// print in student format
void student::print_courseinfo() {
  // iterate through courses and print out
  float my_gpa = 0.0;
  int counter = 1;

  for (pair<string, float> course : courselist) {
    // keep a float of gpa to maintain average
    my_gpa += course.second;

    // print out course, gpa, gpa average
    cout << setw(12) << "Course: " << left << setfill('.') << setw(28)
         << course.first << right << fixed << setprecision(2) << setfill(' ')
         << setw(5) << course.second << " " << my_gpa / counter << endl;

    counter++;
  }
}
