/*
Kaylee Bae
NetID: kbae1
All the functions used in Support.cpp
*/

#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <list>
#include <string>
#include <vector>

using namespace std;

enum label {
  UNKNOWN = -1,
  PERSON = 0,
  FACULTY = 10,
  ASST_PROF = 11,
  ASSOC_PROF = 12,
  PROFESSOR = 13,
  STUDENT = 20,
  FRESHMAN = 21,
  SOPHOMORE = 22,
  JUNIOR = 23,
  SENIOR = 24
};

// base class
class person {
public:
  person();
  friend void operator<<(ostream &out, person &other);
  virtual bool operator<(person &other);
  virtual label get_type();

protected:
  string name;
  label type;
  label category;
  virtual void print_personinfo();
  virtual void print_courseinfo();
};

// class derived from person
class faculty : public person {
public:
  faculty(string new_name, label new_type, label new_category);
  void add_course(string new_course);
  virtual bool operator<(faculty &b); // sort by faculty category
private:
  vector<string> courselist;
  void print_courseinfo();
};

// class derived from person
class student : public person {
public:
  student(string new_name, label new_type, label new_category);
  void add_course(pair<string, float>);
  virtual bool operator<(student &b);

private:
  vector<pair<string, float>> courselist; // unique vector to store gpa
  void print_courseinfo();                // sort by student category
};

#endif