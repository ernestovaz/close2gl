#include "InterfaceWindow.h"

InterfaceWindow::InterfaceWindow() {
    auto pmap = Gtk::make_managed<Gtk::Image>("info.xpm");
    auto label = Gtk::make_managed<Gtk::Label>("cool button");
    label->set_expand(true);

    //Put them in a Box:
    auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 5);
    hbox->append(*pmap);
    hbox->append(*label);

    //And put that Box in the button:
    button.set_child(*hbox);

    set_title("Hello world!");

    button.set_margin(10);
    set_child(button);   set_default_size(200,200);
}
