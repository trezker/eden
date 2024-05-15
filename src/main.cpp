#include <iostream>
#include <vector>
#include <string>
#include <dlfcn.h>
#include "plugin.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

void abort_dialog(char const *format, ...) {
	char str[1024];
	va_list args;
	ALLEGRO_DISPLAY *display;

	va_start(args, format);
	vsnprintf(str, sizeof str, format, args);
	va_end(args);

	if (al_init_native_dialog_addon()) {
		display = al_is_system_installed() ? al_get_current_display() : NULL;
		al_show_native_message_box(display, "Error", "Cannot run", str, NULL, 0);
	}
	else {
		fprintf(stderr, "%s", str);
	}
	exit(1);
}

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
      std::string f = "./plugins/"+name+".so";
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

 	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_EVENT event;

	if (!al_init()) {
		abort_dialog("Could not init Allegro.\n");
	}

	al_init_primitives_addon();
	al_install_mouse();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();

	int width = 1600;
	int height = 900;
	ALLEGRO_MONITOR_INFO info;
	al_get_monitor_info(0, &info);
	width = (info.x2 - info.x1) * 0.8;
	height = (info.y2 - info.y1) * 0.8;

	al_set_new_display_flags(ALLEGRO_OPENGL|ALLEGRO_WINDOWED|ALLEGRO_RESIZABLE);
	al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 24, ALLEGRO_REQUIRE);
	display = al_create_display(width, height);
	if (!display) {
		abort_dialog("Error creating display\n");
	}

	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));

	ALLEGRO_COLOR black = al_map_rgb_f(0, 0, 0);

 	int done = 0;
	while(!done) {
		while(al_get_next_event(queue, &event)) {
			switch(event.type) {
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					done = 1;
					break;
			}
		}
		al_clear_to_color(black);
		
		al_flip_display();
		al_rest(0.001);
	}

	al_destroy_event_queue(queue);
	al_destroy_display(display);
	return EXIT_SUCCESS;
}
