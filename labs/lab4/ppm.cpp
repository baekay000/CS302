
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "ppm.h"

void ppm::read(const string &fname) {
  ifstream fin;

  fin.open(fname.c_str());

  if (!fin.is_open()) {
    cerr << "ppm::read: cannot open " 
         << fname.c_str() 
         << "\n";
    exit(1);
  }

  fin >> magicid >> Ncols >> Nrows >> maxvalue;

  while (fin.get() != '\n') { /* skip */ }

  if (magicid.compare("P6") != 0) {
    cerr << "ppm::read: magicid "
         << magicid
         << " not supported\n";
    exit(1);
  }

  if (maxvalue != 255) {
    cerr << "ppm::read: maxvalue " 
         << maxvalue 
         << " not supported\n";
    exit(1);
  }

  img = new RGB [Nrows*Ncols];

  int nbytes = 3*Ncols*Nrows;
  fin.read((char *)&img[0], nbytes);

  int nread = fin.gcount();

  while (!fin.eof()) {
    fin.read((char *)&img[0], 1024);
    nread += fin.gcount();
  }

  if (nread != nbytes) {
    cerr << "ppm::read: expected " << nbytes
         << " but read " << nread << " bytes\n";
    exit(1);
  }

  fin.close();
}

void ppm::write(const string &fname_in) {
  string fname_out(fname_in);
  
  fname_out.insert(
    fname_out.rfind(".ppm"), "_wmsg"
  );

  ofstream fout(fname_out.c_str(), fstream::trunc);

  if (!fout.good()) {
    cerr << "ppm::write: cannot open " 
         << fname_out.c_str() 
         << "\n";
    exit(1);
  }

  fout << magicid << "\n"
       << Ncols << " " << Nrows << "\n"
       << maxvalue << "\n";

  int nbytes = 3*Ncols*Nrows;
  fout.write((char *)&img[0], nbytes);

  fout.close();
}
