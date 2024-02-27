#include <...>
using namespace std;

#include "ppm.h"

const char EOT = 0x4;    // ASCII: end-of-transmission

rnumgen code from crypto2

void set_pixel_list(...) {
  update crypto2 to start list at pixel 32
}

void permute_pixel_list(ppm &img, vector<int> &pixel_list) {
  from crypto2
}

void encode(...) {
  from crypto1
}

void decode(...) {
  from crypto1
}

void encode_hoffman(...) {
  write this
}

void decode_hoffman(...) {
  write this
}

int main(int argc, char *argv[]) {
  seedvalue = 0
  if optional -seed=N argument
    seedvalue = N

  hoffman = FALSE
  if optional -hoffman argument
    hoffman = TRUE

  ppm img
  img.read(fname)

  declare pixel_list

  srand(seedvalue)

  set_pixel_list(pixel_list)
  if (0 < seedvalue)
    permute_pixel_list(img, pixel_list)

  if (mode = ENCODE) {
    if (hoffman = TRUE)
      encode_hoffman(img, pixel_list)
    else
      encode(img, pixel_list)
    img.write(fname)
  } else
  if (mode = DECODE) {
    if (hoffman = TRUE)
      decode_hoffman(img, pixel_list)
    else
      decode(img, pixel_list)
  }
}
