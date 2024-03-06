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

rnumgen::rnumgen(vector<int> &fcount) {
  CDF.resize(fcount.size());
  partial_sum(fcount.begin(), fcount.end(), CDF.begin());
  transform(CDF.begin(), CDF.end(), CDF.begin(),
            bind2nd(divides<float>(), CDF.back()));
}

int rnumgen::rand() {
  double p = std::rand() / ((double)RAND_MAX + 1);
  if (p == 1.0)
    p -= FLT_EPSILON;
  return upper_bound(CDF.begin(), CDF.end(), p) - CDF.begin();
}

void set_pixel_list(ppm &myppm, vector<int> &my_pixel_list) {
  // produces a list of valid pixel indices
  int size = myppm.get_Npixel();
  for (int i = 0; i < size; i++) {
    my_pixel_list.push_back(i);
  }
}

// // write this
void permute_pixel_list(ppm &img, vector<int> &pixel_list) {
  // compute a histogram of the color values for all pixels
  vector<int> histogram(4096,0);
  // cout << "Got here 0" << endl;

  // int combined_bits;
  for (int k = 0; k < img.get_Npixel(); k++) {
    int combined_bits = 0;

    char modified_bit = img[pixel_list[k]].R << 2 >> 4;
    combined_bits = modified_bit << 8;

    modified_bit = img[pixel_list[k]].G << 2 >> 4;
    combined_bits |= modified_bit << 4;

    modified_bit = img[pixel_list[k]].B << 2 >> 4;
    combined_bits |= modified_bit;

    histogram[combined_bits]++;
  }
  rnumgen myrnumgen = rnumgen(histogram);

  int j;
  for (int i = pixel_list.size() - 1; 0 < i; i--) {
    j = ((myrnumgen.rand() << 12 | myrnumgen.rand()) % (i + 1));
    swap(pixel_list[i], pixel_list[j]);
    // cout << "1 " << endl;
  }

}

// // from crypto 1
void encode(ppm &img, vector<int> &pixel_list) {
  // text is read from stdin and encoded into the image
  char input;
  int counter = 0;
  // way to ignore first 0

  while (cin.get(input)) {
    // if (input == 0x4) {
    //   cout << "DONE!" <<  endl;
    //   break;
    // }
    cout << "Encoding" << input << endl;

    // the 7 bits - first bit
    for (int k = 6; k >= 0; k--) {
      unsigned int modified_bit = (input >> k) & 0x1;

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

      // swhiel loop to get the char
      counter++;
    }

    // do math onto the char to pick ou t pixel you need
    // use switch or if to go through case 0,1,2
    // math to add that pixel into RGB
    // increment counter

    // encode bit 6 into R, 5 into G, 4 into B
    // % 3 -> 0 is B, 2  G1 ,
  }
  for (int k = 6; k >= 0; k--) {
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

    // the 7 bits - first bit
    for (int k = 6; k >= 0; k--) {
      // unsigned int modified_bit = (input >> k) & 0x1;

      int i = counter % 3;

      if (i == 0) {
        input = img[pixel_list[counter]].R &= 0x1;

        // &= 0xFE;
        // img[pixel_list[counter]].R |= modified_bit;

      } else if (i == 1) {
        input = img[pixel_list[counter]].G &= 0x1;
        // img[pixel_list[counter]].G &= 0xFE;
        // img[pixel_list[counter]].G |= modified_bit;

      } else {
        input = img[pixel_list[counter]].B &= 0x1;
        // img[pixel_list[counter]].B &= 0xFE;
        // img[pixel_list[counter]].B |= modified_bit;
      }

      //  mychar = mychar | (input << k);
      mychar |= (input << k);

      counter++;

      // if (mychar == EOT){ cout<< "Done" << endl; break;} else
      // {cout.put(mychar);}
    }
    // cout << "Decoding " << mychar << endl;

    if (mychar == EOT) {
      break;
    } else {
      cout.put(mychar);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4 && argc != 3) {
    cerr << "usage: ./Prog4-Crypto1 -encode|decode [-seed=N] image.ppm" << endl;
    exit(1);
  }

  string fname = argv[argc - 1];

  int seedvalue = 0;
  if (argc == 4) {
    // seedvalue = stoi(string(argv[2]));
    string new_label = strtok(argv[2], "=");
    seedvalue = atoi(strtok(NULL, "="));
    // cout << seedvalue << endl;
  }

  ppm img;
  img.read(fname);

  // Encode or decode mode
  bool encode_mode;

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
    permute_pixel_list(img, pixel_list);
  }

  if (encode_mode) {
    encode(img, pixel_list);
    img.write(fname);
  } else {
    decode(img, pixel_list);
  }
}
