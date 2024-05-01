#include <iostream>
#include <vector>
#include <string>
#include <dlfcn.h>
#include "plugin.h"

class PluginHandle {
  private:
    Plugin* plugin;
    void* dlhandle;
    std::string name;
  public:
    PluginHandle() {
      plugin = NULL;
      dlhandle = NULL;
    }
    ~PluginHandle() {
      delete plugin;
      dlclose(dlhandle);
    }
    bool Load(std::string name) {
      std::string f = "./"+name+".so";
      dlhandle = dlopen(f.c_str(), RTLD_NOW);
      if(!dlhandle) {
        std::cout<<dlerror()<<std::endl;
        return false;
      }
      Plugin* (*c)();
      c = (Plugin*(*)())dlsym(dlhandle, "create");
      char* error = dlerror();
      if(error != NULL) {
        std::cout<<error<<std::endl;
        dlclose(dlhandle);
        dlhandle = NULL;
        return false;
      }
      plugin = (*c)();
      plugin->run();
      return true;
    }
};

class PluginAPI {
  private:
    std::vector<PluginHandle*> pluginhandles;
  public:
    ~PluginAPI() {
    }

};

int main() {
  std::cout<<"Hello"<<std::endl;
  PluginHandle* pluginhandle = new PluginHandle;
  pluginhandle->Load("test");
  delete pluginhandle;
  return 0;
}
