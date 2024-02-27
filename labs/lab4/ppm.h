#ifndef __PPM_H__
#define __PPM_H__

#include <string>

struct RGB {
  RGB() { R=0, G=0, B=0; }
  unsigned char R, G, B;
};

class ppm {
  public:
    ppm() {
      magicid = "P6";
      Nrows = 0;
      Ncols = 0;
      maxvalue = 255;
      img = NULL;
    }

    ~ppm() {
      if (img) 
        delete [] img;
    }

    void read(const std::string &);
    void write(const std::string &);

    RGB &operator[](int i) { return img[i]; }

    int get_Npixel() { return Nrows*Ncols; }

  private:
    std::string magicid;
    int Nrows, Ncols;
    int maxvalue;
    RGB *img;
};

#endif
