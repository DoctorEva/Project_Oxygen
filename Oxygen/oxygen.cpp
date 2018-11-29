
#include "oxygen.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
/* This function opens a file prompter. Uncomment to implement a load game
std::string Window::openfile(std::string prompt)
{
  Gtk::FileChooserDialog dialog(prompt, Gtk::FILE_CHOOSER_ACTION_OPEN);
  //dialog.set_transient_for(*this);
  dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog.add_button("_Open", Gtk::RESPONSE_OK);
  int result = dialog.run();
  std::string ReturnValue;
  switch(result)
    {
    case(Gtk::RESPONSE_OK):
      {
	ReturnValue = dialog.get_filename();
	break;
      }
    case(Gtk::RESPONSE_CANCEL):
      {
	std::cout<<"Canceled File Dialog."<<std::endl;
	break;
      }
    default:
      {
	std::cout<<"File Dialog did something weird."<<std::endl;
	break;
      }
    }
  return ReturnValue;
}
*/
std::vector<std::string> ColonyWindow::read_file(std::string filename)
{
  std::vector<std::string> all;
  std::ifstream input;
  std::string line;
  input.open(filename);
  
  if(!input.is_open())
    {
      std::cout<<"File failed to load"<<std::endl;
      exit(1);
    }
  while(!input.eof())
    {
      input>>line;
      all.push_back(line);
    }
  input.close();
  return all;
}

ColonyWindow::ColonyWindow(std::vector<std::string> HighScoreValues)
{ 
  HighScores = HighScoreValues;
    this->set_title("~~~Oxygen~~~");
    this->set_border_width(10);
    this->resize(400,400);

    // Setting label and button texts
    button_DayStart.add_label("~Start the Day~");

    quit.add_label("~Quit~");
    label_resources.set_text("Label_resources placed here.");

    // Signal Handling.
    button_DayStart.signal_clicked().connect(sigc::mem_fun(*this,&ColonyWindow::on_button_DayStart));
    quit.signal_clicked().connect(sigc::mem_fun(*this,&ColonyWindow::close));

    // Widget Arrangement
    grid.attach(label_resources,   0,0,2,1);
    grid.attach(button_DayStart,   0,1,2,1);
    grid.attach(quit,              1,2,1,1);
    
    grid.show_all();
    add(grid);

    // Startup Dialog
    Gtk::MessageDialog dialog(*this, "You are in charge of leading a small space colony!\n", false, Gtk::MESSAGE_INFO);
    dialog.set_secondary_text("To survive, you must produce power and create enough oxygen to breathe and grow, good luck Boss!\n\nRead the README for a tutorial on mechanics!");
    dialog.run();

    coal = 0;
    Person.CoalPtr = &coal;
    Person.add_coal(10);
}
ColonyWindow::~ColonyWindow()
{
    
}
//Button Functionality.
void ColonyWindow::on_button_DayStart()
{
  std::cout<<"DayStart button placeholder."<<std::endl;
}


void Colonist::add_coal(int amount)
{
  *CoalPtr += amount;
  //printf("coal added, now there is %d\n", *CoalPtr);
}
