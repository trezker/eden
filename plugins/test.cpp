#include <iostream>
#include "../src/module.h"

class Test: public Module {
  public:
    virtual ~Test() {}
    virtual void run() {
      std::cout<<"Test module"<<std::endl;
    }
};

extern "C" Module* create() {
  return new Test;
}
