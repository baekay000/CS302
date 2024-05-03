#ifndef __SUPPORT_H__
#define __SUPPORT_H__

// #include <...>
#include <list>
#include <string>
#include <vector>

// These files are empty apart from an enum whose values must use to define your
// data. Your job is define and implement the following three classes: person,
// faculty, and student.

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
  // person(string new_name, label new_type, label new_category);

  // from piazza
  // void add_courses();
  virtual string read_name();
  virtual label read_category();
  // virtual ~person();

  // friend operator<< ( );
  virtual bool operator<(person &other);
  // virtual void get_type();
  // virtual void get_category();

protected:
  string name;
  label type;
  label category;
  // #print_personinfo ( )=0
  // #print_courseinfo ( )=0
  // + X means X is a public member,
  //     #X means X is a protected member,
  //     and-X means X is a private member.
};

// In case you wonder, then the solution code feeds name and category to the
// faculty/student constructor and uses a public member function to add courses.
// Other public member functions allow reading the name and the category when
// needed.

// class derived from person
class faculty : public person {
public:
  faculty();
  faculty(string new_name, label new_type, label new_category);
  void add_course(string new_course);
  // bool operator<(string &other); // for courselist I assume
private:
  vector<string> courselist;
};
// + operator<() +
//     add_course() - courselist[] - print_personinfo() -
//     print_courseinfo()

// class derived from person
class student : public person {
public:
  // student();
  student(string new_name, label new_type, label new_category);
  // bool operator<();
  // void add_course();

private:
  // list courselist[];
  // -print_personinfo ( )
  // -print_courseinfo ( )
};

#endif