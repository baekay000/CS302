#include <iostream>
using namespace std;

#include "Support.h"

// person class member function implemention

// person::person(string new_name, label new_type, label new_category) {
//   name = new_name;
//   type = new_type;
//   category = new_category;
// };

// person::~person(){
//   ;
// };

bool person::operator<(person &b) { return name < b.name; }

string person::read_name() { return name; };
label person::read_category() { return category; };

// void person::set_name(string &my_name) { name = my_name; };

// person::~person() {}

// int person::get_type() {
// using
// }

// faculty class member function implemention
// bool faculty::operator<(string &other){
//   // return this. < other;
// };

void faculty::add_course(string new_course) {
  courselist.push_back(new_course);
}

person::person(){
  ;
}

faculty::faculty(string new_name, label new_type, label new_category){
  name = new_name;
  type = new_type;
  category = new_category;
}

// student class member function implemention
student::student(string new_name, label new_type, label new_category){
  name = new_name;
  type = new_type;
  category = new_category;
}
