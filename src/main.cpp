#include <iostream>
#include <vector>
#include <string>
#include <dlfcn.h>
#include "module.h"

class ModuleHandle {
  private:
    Module* module;
    void* dlhandle;
    std::string name;
  public:
    ModuleHandle() {
      module = NULL;
      dlhandle = NULL;
    }
    ~ModuleHandle() {
      delete module;
      dlclose(dlhandle);
    }
    bool Load(std::string name) {
      std::string f = "./"+name+".so";
      dlhandle = dlopen(f.c_str(), RTLD_NOW);
      if(!dlhandle) {
        std::cout<<dlerror()<<std::endl;
        return false;
      }
      Module* (*c)();
      c = (Module*(*)())dlsym(dlhandle, "create");
      char* error = dlerror();
      if(error != NULL) {
        std::cout<<error<<std::endl;
        dlclose(dlhandle);
        dlhandle = NULL;
        return false;
      }
      module = (*c)();
      module->run();
      return true;
    }
};

class ModuleAPI {
  private:
    std::vector<ModuleHandle*> modulehandles;
  public:
    ~ModuleAPI() {
    }

};

int main() {
  std::cout<<"Hello"<<std::endl;
  ModuleHandle* modulehandle = new ModuleHandle;
  modulehandle->Load("test");
  delete modulehandle;
  return 0;
}
