#include "UserInterface.h"

#include "InterfaceWindow.h"

#include <cstddef>

UserInterface::UserInterface() {
    auto application = Gtk::Application::create("hello.world");
    interfaceThread = new thread(
            &Gtk::Application::make_window_and_run<InterfaceWindow>,
            application,
            0,
            nullptr 
    );
}

UserInterface::~UserInterface() {
    interfaceThread->join();
}
