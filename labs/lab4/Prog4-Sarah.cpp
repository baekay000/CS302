#include<iostream>
#include<vector>
#include<iomanip>
#include<sstream>

#include "ppm.h"

using namespace std;

const char EOT = 0x4;    // ASCII: end-of-transmission

// the set_pixel_list() function produces a list of valid pixel indices, namely, 0 thru img.get_Npixel()-1. These are conveniently stored in a vector array.
void set_pixel_list(ppm img, vector<int> &pixel_list) {
  for(int i=0;i<img.get_Npixel()-1;i++) { 
    pixel_list.push_back(i); 
  }
}

/*void encode(ppm &img, vector<int> &pixel_list) {
  if(comments){cout<<"Start of encode"<<endl;}
  stringstream ss;
  string message;

  while (getline(cin, message)) {ss << message << endl; }
  // encode into image
  char letter; 
  int pixel_count = 0;
  
  while(ss.get(letter)){
    cout<<letter<<endl;
    for(int k=6;k>=0;k--){
      int index=pixel_list[pixel_count];
      int rgb_bit = pixel_count % 3;
      // get one bit out of the message
      char bit = (letter<<k) & 0x1;

      // rgb_bit=pixel_count%3;
      //cout<<"index="<<index<<endl;
      //cout<<"R="<<img[index].R<<" G="<<img[index].G<<" B="<<img[index].B<<endl;
      if (rgb_bit == 0) {
        img[index].R &= 0xFE; 
        img[index].R |= bit;
      } else if (rgb_bit == 1) {
        img[index].G &= 0xFE; 
        img[index].G |= bit;
      } else if (rgb_bit == 2) {
        img[index].B &= 0xfe; 
        img[index].B |= bit;
      }
      pixel_count++;

      // if(rgb_bit==0){ cout<<"A"<<endl; img[index].R &= 0xfe; img[index].R |= bit; }
      // else if(rgb_bit==1){ cout<<"B"<<endl; img[index].G &= 0xfe; img[index].G |= bit; }
      // else { cout<<"C"<<endl; img[index].B &= 0xfe; img[index].B |= bit; }
      // pixel_count++;
    }
  }
  // add EOT character
  letter=EOT;
    for(int k=6;k>=0;k--){
      int index=pixel_list[pixel_count];
      int rgb_bit=pixel_count % 3;
      // get one bit out of the message
      char bit = (letter<<k) & 0x1;

      // rgb_bit=pixel_count%3;
      //cout<<"index="<<index<<endl;
      //cout<<"R="<<img[index].R<<" G="<<img[index].G<<" B="<<img[index].B<<endl;
      if (rgb_bit == 0) {
        img[index].R &= 0xFE; 
        img[index].R |= bit;
      } else if (rgb_bit == 1) {
        img[index].G &= 0xFE; 
        img[index].G |= bit;
      } else if (rgb_bit == 2) {
        img[index].B &= 0xfe; 
        img[index].B |= bit;
      }
      pixel_count++;



      // // pick R G or B
      // int index=pixel_list[pixel_count];
      // int rgb_bit=pixel_count%3;
      // // get one bit out of the message
      // char bit;
      // bit = letter<<k; bit &= 0x1;
      // if(rgb_bit==0){ img[index].R &= 0xfe; img[index].R |= bit; }
      // else if(rgb_bit==1){ img[index].G &= 0xfe; img[index].G |= bit; }
      // else { img[index].B &= 0xfe; img[index].B |= bit; }
      // pixel_count++;
    }
    if(comments){cout<<"End of encode"<<endl;}
}*/

void encode(ppm &img, vector<int> &pixel_list) {
    string text;
    stringstream ss;
    while (getline(cin, text)) { ss << text << endl; }

    // read and encode the text one character at a time including all white space
    char character;
    int pixel_count = 0;
    while (ss.get(character)) {
        for (int j = 6; j >= 0; j--) {
            int pixel_index = pixel_list[pixel_count];
            int RGB_bit = pixel_count % 3;

            // will move j to the LSB and set all bits but LSB to 0
            char bit = (character >> j) & 0x1;

            if (RGB_bit == 0) {
                // red bit
                img[pixel_index].R &= 0xFE;
                img[pixel_index].R |= bit;
            } else if (RGB_bit == 1) {
                // green bit
                img[pixel_index].G &= 0xFE;
                img[pixel_index].G |= bit;
            } else if (RGB_bit == 2) {
                // blue bit
                img[pixel_index].B &= 0xFE;
                img[pixel_index].B |= bit;
            };
            pixel_count++;
        };
    };
    // After end of ss, set char as EOT character
    character = EOT;
    for (int k = 6; k >= 0; k--) {
        int pixel_index = pixel_list[pixel_count];
        int RGB_bit = pixel_count % 3;

        // will move k to the LSB and set all bits but LSB to 0
        char bit = (character >> k) & 0x1;

        if (RGB_bit == 0) {
            // red bit
            img[pixel_index].R &= 0xfe; 
            img[pixel_index].R |= bit;
        } else if (RGB_bit == 1) {
            // green bit
            img[pixel_index].G &= 0xfe;
            img[pixel_index].G |= bit;
        } else if (RGB_bit == 2) {
            // blue bit
            img[pixel_index].B &= 0xfe;
            img[pixel_index].B |= bit;
        };
        pixel_count++;
    };
};

void decode(ppm img, vector<int> list) {
  //if(comments){ cout<<"Start of decode"<<endl; }
  RGB pixel; char c; bool notEOT=true; int index;
  int i=0;
  while(notEOT){
    c=0x0;
    for(int j=0;j<7;j++){
      index=list[i];
      c=c>>1;
      if(i%3==0){ c = c | (img[index].R & 0x1); }
      else if(i%3==1){ c = c | (img[index].G & 0x1); }
      else { c = c | (img[index].B & 0x1); }
      i++;
    }
    if(c==EOT){notEOT=false;}
    cout<<(char)(c);
  }
  //if(comments){ cout<<"End of decode"<<endl; }
}

// ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
int main(int argc, char *argv[]) {
  bool bad=false;
  int mode=0; string fname="";
  // command line reading
  if(argc==3 || argc==4){
      if(argv[1]==(string)"-encode"){ mode=1; }
      else if(argv[1]==(string)"-decode"){ mode=2; }
      else {bad=true;}
      if(argc==4){
        if(argv[2]==(string)"-verbose"){ }
        else {bad=true;}
        fname=argv[3];
      }
      else {
        fname=argv[2];
      }
  }

  /*if(comments){
    cout<<"Mode = "<<mode<<endl;
    cout<<"Verbose = "<<verbose<<endl;
    cout<<"File = "<<fname<<endl;
    cout<<"Bad = "<<bad<<endl;
  }*/
  if(mode==0){bad=true;}
  if(bad){ cerr<<"usage: ./Prog4-crypto1 -encode|decode [-verbose] image.ppm"<<endl; return 0; }
  // ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  ppm img;
  img.read(fname);

  vector<int> pixel_list;

  set_pixel_list(img, pixel_list);

  if(mode==1){ encode(img, pixel_list); img.write(fname); }
  else if(mode==2){ decode(img, pixel_list); }
}