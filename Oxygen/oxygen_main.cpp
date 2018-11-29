#include "oxygen.h"
#include <gtkmm/application.h>
#include <iostream>
#include <vector>

using namespace std;
//Project Oxygen, Thomas Tran, Andrea Munoz, Micheal Ly, Diego Vester


int main(int argc, char** argv) {
  
    auto app = Gtk::Application::create(argc,argv, "uta.edu");
    vector<string> HighScores = ColonyWindow::read_file("High Scores List");
    // Reads in High Scores List for the program.
    
    ColonyWindow w(HighScores);
    cout<<"Window Launched."<<endl;
    return app->run(w);

    
    return 0;
}


