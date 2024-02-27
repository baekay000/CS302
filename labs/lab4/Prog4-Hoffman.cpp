#include <...>

write this based on class08_rnumgen_handout.pdf

class hoffman {
  struct heapnode { ... }

  public:
    hoffman();
    ~hoffman();

    string &encode(char);
    char decode(char);

  private:
    data and function members needed 
};

int main(int argc, char *argv[]) {
  extract and process commandline arguments
  Prog4-hoffman -encode|decode [-seed=N]

  srand(seedvalue);

  hoffman secret_cipher;

  if (mode = ENCODE) {
    char c;
    while (cin.get(c)) 
      cout << secret_cipher.encode(c);
  } else 
  if (mode = DECODE) {
    char c;
    while (cin.get(c)) {
      c = secret_cipher.decode(c);
      if (c != -1)
        cout << c;
    }
  }

  return 0;
}
