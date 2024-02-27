#include <...>
using namespace std;

#include "ppm.h"

const char EOT = 0x4;    // ASCII: end-of-transmission

incorporate rnumgen code from class08_rnumgen_handout.pdf

void set_pixel_list(...) {
  from crypto1
}

void permute_pixel_list(ppm &img, vector<int> &pixel_list) {
  write this
}

void encode(...) {
  from crypto1
}

void decode(...) {
  from crypto1
}

int main(int argc, char *argv[]) {
  seedvalue = 0
  if optional -seed=N argument
    seedvalue = N

  ppm img
  img.read(fname)

  declare pixel_list

  srand(seedvalue)

  set_pixel_list(pixel_list)
  if (0 < seedvalue)
    permute_pixel_list(img, pixel_list)

  if (mode = ENCODE) {
    encode(img, pixel_list)
    img.write(fname)
  } else
  if (mode = DECODE) 
    decode(img, pixel_list)
}
