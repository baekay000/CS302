#include <bitset>
#include <fstream>
#include <iostream>
#include <vector>
// #include <is

using namespace std;

#include "ppm.h"

const char EOT = 0x4; // ASCII: end-of-transmission

void set_pixel_list(ppm &myppm, vector<int> &my_pixel_list) {
  // produces a list of valid pixel indices
  int size = myppm.get_Npixel();
  for (int i = 0; i < size; i++) {
    my_pixel_list.push_back(i);
  }
}

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
      unsigned int modified_bit = (input >> k);

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

  // the 7 bits - first bit
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

    // swhiel loop to get the char
    counter++;
  }
}

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
  // parse command line arguments
  if (argc != 3) {
    cerr << "usage: ./Prog4-Crypto1 -encode|decode image.ppm" << endl;
    exit(1);
  }
  string fname = argv[2];
  ppm img;
  img.read(fname);

  // Encode or decode mode
  bool encode_mode;
  if ((string)argv[1] == "-encode") {
    encode_mode = true;
  } else if ((string)argv[1] == "-decode") {
    encode_mode = false;
  } else {
    cerr << "usage: ./Prog4-Crypto1 -encode|decode image.ppm" << endl;
    exit(1);
  }

  vector<int> pixel_list;

  set_pixel_list(img, pixel_list);

  if (encode_mode) {
    encode(img, pixel_list);
    img.write(fname);
  } else {
    decode(img, pixel_list);
  }
}
