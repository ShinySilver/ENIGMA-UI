#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <string>

namespace Utils{
  class Settings{ // static class. Yeah.
    public:
      static void setFlag(std::string flagName, unsigned char value);
      static short int getFlag(std::string flagName);
      typedef struct {
        std::string id;
        unsigned char value;
      } DictionnaryEntry;
    private:
      static std::vector<DictionnaryEntry> dictionnary;
  };
}

#endif /* end of include guard: SETTINGS_H */