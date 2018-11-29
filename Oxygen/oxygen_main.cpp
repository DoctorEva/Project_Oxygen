#include "oxygen.h"
#include <gtkmm/application.h>
#include <iostream>
#include <vector>

using namespace std;
//Thomas Tran, 1000901761


int main(int argc, char** argv) {
  
    auto app = Gtk::Application::create(argc,argv, "uta.edu");
    //string filename = Window::openfile("Please select your save file."); // Uncomment if we want a save/load ability.
    vector<string> HighScores = ColonyWindow::read_file("High Scores List"); // Reads in High Scores List for the program.
    
    ColonyWindow w(HighScores);
    cout<<"Window Launched."<<endl;
    return app->run(w);

    
    return 0;
}


