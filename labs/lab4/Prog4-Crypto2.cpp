/* Kaylee Bae
Lab 3: Program that encodes a PPM image with ASCII text, making it a form of
steganography.
*/

#include <algorithm>
#include <bitset>
#include <cfloat>
#include <cstring>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

#include "ppm.h"

const char EOT = 0x4; // ASCII: end-of-transmission

// incorporate rnumgen code from class08_rnumgen_handout.pdf
class rnumgen {
public:
  rnumgen(vector<int> &);
  int rand();

private:
  vector<float> CDF;
};

// Copied from handout
rnumgen::rnumgen(vector<int> &fcount) {
  CDF.resize(fcount.size());
  partial_sum(fcount.begin(), fcount.end(), CDF.begin());
  transform(CDF.begin(), CDF.end(), CDF.begin(),
            bind2nd(divides<float>(), CDF.back()));
}

// Copied from handout
int rnumgen::rand() {
  double p = std::rand() / ((double)RAND_MAX + 1);
  if (p == 1.0)
    p -= FLT_EPSILON;
  return upper_bound(CDF.begin(), CDF.end(), p) - CDF.begin();
}

// Crypto 1
void set_pixel_list(ppm &myppm, vector<int> &my_pixel_list) {
  // produces a list of valid pixel indices
  int size = myppm.get_Npixel();
  for (int i = 0; i < size; i++) {
    my_pixel_list.push_back(i);
  }
}

// adds randomness to the pixel target through use of permute
void permute_pixel_list(ppm &img, vector<int> &pixel_list) {
  // compute a histogram of the color values for all pixels
  vector<int> histogram(4096, 0);

  // iterates through the red, green, and blue bits to grab 2-5
  for (int k = 0; k < img.get_Npixel(); k++) {
    int combined_bits = 0;

    // removes first two and last two bits
    char modified_bit = img[pixel_list[k]].R << 2 >> 4;

    // moves modified bit to the front of the combined
    combined_bits = modified_bit << 8;

    modified_bit = img[pixel_list[k]].G << 2 >> 4;
    // moves modified bit to the middle
    combined_bits |= modified_bit << 4;

    modified_bit = img[pixel_list[k]].B << 2 >> 4;
    combined_bits |= modified_bit;

    // increments the histogram to keep track of the frequency of bits
    histogram[combined_bits]++;
  }

  // create new rnumgen
  rnumgen myrnumgen = rnumgen(histogram);

  int j;
  for (int i = pixel_list.size() - 1; 0 < i; i--) {
    // combine two 12-bits using rand() in the rnumgen class
    j = ((myrnumgen.rand() << 12 | myrnumgen.rand()) % (i + 1));
    swap(pixel_list[i], pixel_list[j]);
  }
}

// from crypto 1
void encode(ppm &img, vector<int> &pixel_list) {
  char input;
  int counter = 0;

  while (cin.get(input)) {

    // iterates through the 8 bits minus the first bit (which is always 0)
    for (int k = 6; k >= 0; k--) {
      unsigned int modified_bit = (input >> k) & 0x1;

      // uses mod to "randomize" if the RGB gets the pixel
      int i = counter % 3;

      // uses if statement to go through case 0, 1, or 2
      if (i == 0) {
        img[pixel_list[counter]].R &= 0xFE;
        img[pixel_list[counter]].R |= modified_bit;

      } else if (i == 1) {
        img[pixel_list[counter]].G &= 0xFE;
        img[pixel_list[counter]].G |= modified_bit;

      } else {
        img[pixel_list[counter]].B &= 0xFE;
        img[pixel_list[counter]].B |= modified_bit;
      }

      counter++;
    }
  }

  // after the message, encode an EOT to know when to stop decoding a message
  for (int k = 6; k >= 0; k--) {

    // same logic as previous, using EOT character
    unsigned int modified_bit = (EOT >> k) & 0x1;

    int i = counter % 3;

    if (i == 0) {
      img[pixel_list[counter]].R &= 0xFE;
      img[pixel_list[counter]].R |= modified_bit;

    } else if (i == 1) {
      img[pixel_list[counter]].G &= 0xFE;
      img[pixel_list[counter]].G |= modified_bit;

    } else {
      img[pixel_list[counter]].B &= 0xFE;
      img[pixel_list[counter]].B |= modified_bit;
    }
    counter++;
  }
}

// crypto 1
void decode(ppm &img, vector<int> &pixel_list) {
  unsigned int input;
  int counter = 0;
  char mychar;

  while (1) {
    mychar = 0;

    // the 7 bits minus the first bit
    for (int k = 6; k >= 0; k--) {

      // decodes using same logic as encode
      int i = counter % 3;

      if (i == 0) {
        // grabs the last bit of the red
        input = img[pixel_list[counter]].R &= 0x1;

      } else if (i == 1) {
        // last bit of the green
        input = img[pixel_list[counter]].G &= 0x1;

      } else {
        // last bit of the blue
        input = img[pixel_list[counter]].B &= 0x1;
      }

      // put the char together by shifting the input
      mychar |= (input << k);

      counter++;
    }

    if (mychar == EOT) {
      // the only way to exit decoding is to reach an EOT
      break;
    } else {
      // put char in cout for printing
      cout.put(mychar);
    }
  }
}

int main(int argc, char *argv[]) {

  // checks if the number of command line arguments are valid
  if (argc != 4 && argc != 3) {
    cerr << "usage: ./Prog4-Crypto1 -encode|decode [-seed=N] image.ppm" << endl;
    exit(1);
  }

  // stores file
  string fname = argv[argc - 1];

  // sets random seedvalue to 0, unless seed value is provided
  int seedvalue = 0;
  if (argc == 4) {
    // written using Dr. Gregor's example code on Canvas
    string new_label = strtok(argv[2], "=");
    seedvalue = atoi(strtok(NULL, "="));
  }

  ppm img;
  img.read(fname);

  // Encode or decode mode
  bool encode_mode;

  // parses commands line arguments to check if the mode is decode or encode
  if ((string)argv[1] == "-encode") {
    encode_mode = true;
  } else if ((string)argv[1] == "-decode") {
    encode_mode = false;
  } else {
    cerr << "usage: ./Prog4-Crypto2 -encode|decode [-seed=N] image.ppm" << endl;
    exit(1);
  }
  vector<int> pixel_list;

  srand(seedvalue);
  set_pixel_list(img, pixel_list);

  if (0 < seedvalue) {
    // calls permutation function and passes in parameters
    permute_pixel_list(img, pixel_list);
  }

  // encodes and writes, or decodes to provide a message
  if (encode_mode) {
    encode(img, pixel_list);
    img.write(fname);
  } else {
    decode(img, pixel_list);
  }
}
