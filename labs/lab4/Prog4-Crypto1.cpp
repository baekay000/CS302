#include <iostream>

using namespace std;

#include "ppm.h"

const char EOT = 0x4;    // ASCII: end-of-transmission

void set_pixel_list(...) {
  write this
}

void encode(...) {
  write this
}

void decode(...) {
  write this
}

int main(int argc, char *argv[]) {
  // parse command line arguments
  if (argc != 3) {
    cerr << "usage: ./Prog4-Crypto1 -encode|decode image.ppm" << endl;
    exit(1);
  } 
  string fname = argv[2];
  ppm img;
  img.read(fname);

  // TODO: declare pixel_list
  // vector <> pixel_list;

  set_pixel_list(pixel_list);

  if (mode = ENCODE) {
    encode(img, pixel_list)
    img.write(fname)
  }
  else
  if (mode = DECODE) 
    decode(img, pixel_list)
}
