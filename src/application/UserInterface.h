#pragma once

#include <gtkmm.h>
#include <thread>

using std::thread;

class UserInterface {
private:
    Gtk::Application* application;
    thread* interfaceThread;

public:
    UserInterface();
    ~UserInterface();
};


