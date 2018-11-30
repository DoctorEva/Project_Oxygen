
#include "oxygen.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

//____________ Colony Window Class Implementations___________________
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
    //____________________OXYGEN STUFF___________________
    coal = 0;
    oxygen = 100;
    raw_metal = 0;
    ref_metal = 0;
    day = 0;
    Batteries BB(&oxygen, &raw_metal, &ref_metal);
    Generator FirstGenerator(&oxygen, &BB, LOW_EFF_OUTPUT);
    Generators.push_back(FirstGenerator);
    // Create the first colonist
    // Begin the game running thing.
}
ColonyWindow::~ColonyWindow()
{
    
}
//Button Functionality.
void ColonyWindow::on_button_DayStart()
{
  std::cout<<"DayStart button placeholder."<<std::endl;
}
void ColonyWindow::end_game() // Ends the game if a loss or win condition is met.
{
  if(oxygen < 0) // Loss
    {
      Gtk::MessageDialog dialog(*this, "Game Over: Lost Colony\n", false, Gtk::MESSAGE_INFO);
      dialog.set_secondary_text("Your oxygen was depleted. Remember that burning generators and additional people consume oxygen.");
      dialog.run();
      ColonyWindow::close();
    }
  else if(Colonists.size() == 0) // Loss
    {
      Gtk::MessageDialog dialog(*this, "Game Over: Abandoned Colony\n", false, Gtk::MESSAGE_INFO);
      dialog.set_secondary_text("Your colonists all left. Remember that stressed colonists become unproductive and need a rest.");
      dialog.run();
      ColonyWindow::close();
    }
  if(Colonists.size() == COLONISTS_TO_WIN) // Win
    {
      Gtk::MessageDialog dialog(*this, "INDEPENDENCE: Victory!\n", false, Gtk::MESSAGE_INFO);
      dialog.set_secondary_text("Your colony is now able to run without your help, well done!\nDays to independence: %d.",day);
      dialog.run();
      std::string new_record = player_name + "_"+std::to_string(day);
      HighScores.push_back(new_record);
      //To do - output sorted High Scores to screen..
      //To do - output High Scores to file.
      ColonyWindow::close();
    }
}
//____________________Battery Class Implementation________________
Batteries::Batteries(int* oxygen, int* raw, int* ref)
{
  OxygenPtr = oxygen;
  RawPtr = raw;
  RefPtr = ref;

  MaxPower = CAPACITY_PER_BATTERY;
  Power = 0;
}
void Batteries::refine_metal(int amount) // Amount is the amount of power to spend.
{

}
void Batteries::hydrolysis(int amount) // Amount is the amount of power to spend.
{

}
//___________________Generaor Class Implementation________________
Generator::Generator(int* oxygen, Batteries* PowerOut, int efficiency)
{
  OxygenPtr = oxygen;
  this->efficiency = efficiency;
  PowerGrid = PowerOut;

  internal_coal = 0;
}
void Generator::charge_batteries()
{
  if(internal_coal>0)
    {
      // To do: Consume O2 and internal coal to make power.
    }
}


//____________________Colonist Class Implementation_______________
Colonist::Colonist(std::string name, int* Coal, int* Oxygen, std::vector<Generator*>* GeneratorList) // Tommy
{
  CoalPtr = Coal;
  OxygenPtr = Oxygen;
  stress = 0;
  this->name = name;
  GenAccess = GeneratorList;
}
Colonist::~Colonist()
{

}
Generator* Colonist::find_gen() // Returns the most empty generator.
{
  int i, min;
  Generator* ret = (*GenAccess)[0]; // Game starts with 1 generator, so assume there is always one.
  for (i=1, min = ret->internal_coal;i<(*GenAccess).size();i++)
    {
      if((*GenAccess)[i]->internal_coal < min)
	{
	  ret = (*GenAccess)[i];
	  min = ret->internal_coal;
	}
    }
  return ret;
}
void Colonist::add_coal() // Tommy
{
  Generator* targetGenerator = find_gen();
  *CoalPtr -= ADD_COAL_AMOUNT;
  targetGenerator->internal_coal += ADD_COAL_AMOUNT;
  stress += STRESS_ON_WORK;
}
//__Engineer
Engineer::Engineer(std::string name, int* Coal, int* Oxygen,std::vector<Generator*>* GeneratorList, int* raw, int* ref):Colonist(name, Coal, Oxygen, GeneratorList)
{
  rawPtr = raw;
  refPtr = ref;
}
void Engineer::do_work()
{

}
//__Miner
Miner::Miner(std::string name, int* Coal, int* Oxygen,std::vector<Generator*>* GeneratorList, int* raw):Colonist(name, Coal, Oxygen, GeneratorList)
{
  rawPtr = raw;
}
void Miner::do_work()
{

}
//__Caretaker
Caretaker::Caretaker(std::string name, int* Coal, int* Oxygen,std::vector<Generator*>* GeneratorList, std::vector<Colonist*>* PeopleList):Colonist(name, Coal, Oxygen, GeneratorList)
{
  PatientList = PeopleList;
}
void Caretaker::do_work()
{

}
