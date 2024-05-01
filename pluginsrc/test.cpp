#include <iostream>
#include "../src/plugin.h"

class Test: public Plugin {
  public:
    virtual ~Test() {}
    virtual void run() {
      std::cout<<"Test plugin"<<std::endl;
    }
};

extern "C" Plugin* create() {
  return new Test;
}
