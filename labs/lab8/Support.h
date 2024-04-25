#ifndef __SUPPORT_H__ 
#define __SUPPORT_H__

#include <...>
using namespace std;

enum label { UNKNOWN=-1, PERSON=0,
             FACULTY=10, ASST_PROF=11, ASSOC_PROF=12, PROFESSOR=13,
             STUDENT=20, FRESHMAN=21, SOPHOMORE=22, JUNIOR=23, SENIOR=24 };

class person -- base class

class faculty -- class derived from person

class student -- class derived from person

#endif

