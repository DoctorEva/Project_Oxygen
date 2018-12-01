
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
    this->set_title("~~~Project Oxygen~~~");
    this->set_border_width(10);
    this->resize(400,400);

    // Setting label and button texts
    worker_start.add_label("~Issue Orders");
    quit.add_label("~Quit~");
    label_resources.set_text("Label_resources placed here.");

    // Signal Handling.
    worker_start.signal_clicked().connect(sigc::mem_fun(*this,&ColonyWindow::on_worker_start));
    quit.signal_clicked().connect(sigc::mem_fun(*this,&ColonyWindow::close));

    // Widget Arrangement
    grid.attach(label_resources,   0,0,2,1);
    grid.attach(worker_start,      0,2,1,1);
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
    day = 1;
    Batteries BB(&oxygen, &raw_metal, &ref_metal);
    Generator FirstGenerator(&oxygen, &BB, LOW_EFF_OUTPUT);
    Generators.push_back(FirstGenerator);
    
    Miner Miner(&coal, &oxygen, &Generators, &raw_metal);
    Engineer Engineer(&coal, &oxygen, &Generators, &raw_metal, &ref_metal);
    Caretaker Caretaker(&coal, &oxygen, &Generators, &Colonists);


    create_colonist(); // First Colonist.
    // Begin the game running thing.
    while(1)
      {
	end_game(); // Try to end the game upon day start
	oxygen -= Colonists.size() * COLONISTS_DAILY_OXYGEN;
	//Ask if a new colonist is wanted
	Gtk::MessageDialog prompt_for_colonist(*this, "Do you want to call a new colonist today?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
	if(prompt_for_colonist.run() == Gtk::RESPONSE_YES)
	  {
	    if(day>1)
	      create_colonist();
	    stress_all_colonists(-5);
	  }
	// Removed stressed colonists
	for(int i=0;i<Colonists.size();i++)
	  {
	    if(Colonists[i]->stress >= 100)
	      {
		Colonists.erase(Colonists.begin()+i);
		stress_all_colonists(5);
		i = 0; // Colonists stresses changed, so have to check everyone again.
	      }
	  }
	// Let the user do a GUI thing to assign job assignments. Once they finish that,
	// they can press the worker start button to have every colonist do thier thing.
      }
}
ColonyWindow::~ColonyWindow()
{

}
void ColonyWindow::end_game() // Ends the game if a loss or win condition is met.
{
  if(oxygen < 0) // Loss
    {
      Gtk::MessageDialog dialog(*this, "Game Over: Lost Colony\n", false, Gtk::MESSAGE_INFO);
      dialog.set_secondary_text("Your oxygen was depleted. Remember that burning generators and additional people consume oxygen.");
      dialog.run();
      exit(0);
    }
  else if(Colonists.size() == 0) // Loss
    {
      Gtk::MessageDialog dialog(*this, "Game Over: Abandoned Colony\n", false, Gtk::MESSAGE_INFO);
      dialog.set_secondary_text("Your colonists all left. Remember that stressed colonists become unproductive and need a rest.");
      dialog.run();
      exit(0);
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
      exit(0);
    }
}
void ColonyWindow::create_colonist()
{
  std::cout<<"Create colonist called"<<std::endl;
  Gtk::Window w;
  Gtk::Dialog *dialog = new Gtk::Dialog();
  dialog->set_transient_for(w);
  dialog->set_title("Calling a Colonist");
  dialog->resize(300,100);

  Gtk::Label *label=new Gtk::Label("Enter Colonist name:");
  dialog->get_content_area()->pack_start(*label);
  label->show();

  Gtk::Entry *entry = new Gtk::Entry();
  entry->set_text("Lil'Fadihy");
  entry->show();
  dialog->get_vbox()->pack_start(*entry);

  dialog->add_button("Miner", 0);
  dialog->add_button("Engineer",1);
  dialog->add_button("Caretaker",2);
  int choice = dialog->run();
  int i = 1;
  Colonist* NewGuy;
  NewGuy->name = entry->get_text();
  
  switch(choice)
    {
    case 0:
      // Miner
      NewGuy = new Miner(&coal, &oxygen, &Generators, &raw_metal);
      break;
    case 1:
      // Engineer
      NewGuy = new Engineer(&coal, &oxygen, &Generators, &raw_metal, &ref_metal);
      break;
    case 2:
      // Caretaker
      NewGuy = new Caretaker(&coal, &oxygen, &Generators, &Colonists);
      break;
    default: // Unexpected selection
      i = 0;
    }
  if(i)
    Colonists.push_back(NewGuy);
  dialog->close();
  delete dialog;
  delete label;
  delete entry;
}
void ColonyWindow::stress_all_colonists(int amount)
{
  for(int i = 0; i<Colonists.size();i++)
    {
      Colonists[i]->stress -= 5;
      if(Colonists[i]->stress < 0)
	{
	  Colonists[i]->stress = 0; // Colonist stress cannot be negative.
	}
    }
}
//Button Functionality.
void ColonyWindow::on_worker_start()
{

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
void Batteries::refine_metal(int amount) // Amount is the amount of power to spend. //Michael
{
  for(int i = 0; i<amount; i++)
    {
      if(Power != 0)
	{
	  Power--;
	  *RefPtr++;
	  *RawPtr -= 2;
	}
      else
	{
	  std::cout<<"Do not have enough power to refine material."<<std::endl;
	  break;
	}
    }
}
void Batteries::hydrolysis(int amount) // Amount is the amount of power to spend. //Michael
{
  for(int i = 0; i<amount; i++)
    {
      if(Power != 0)
	{
	  Power--;
	  *OxygenPtr++;
	}
      else
	{
	  std::cout<<"Do not have enough power to refine material."<<std::endl;
	  break;
	}
    }
}
//___________________Generator Class Implementation________________
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
Colonist::Colonist(int* Coal, int* Oxygen, std::vector<Generator>* GeneratorList) // Tommy
{
  CoalPtr = Coal;
  OxygenPtr = Oxygen;
  stress = 0;
  GenAccess = GeneratorList;
}
Colonist::~Colonist()
{

}
Generator* Colonist::find_gen() // Returns the most empty generator.
{
  int i, min;
  Generator* ret = &(*GenAccess)[0]; // Game starts with 1 generator, so assume there is always one.
  for (i=1, min = ret->internal_coal;i<(*GenAccess).size();i++)
    {
      if((*GenAccess)[i].internal_coal < min)
	{
	  ret = &(*GenAccess)[i];
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
void Colonist::rest()
{
  stress -= COLONIST_REST_AMOUNT;
}

//__Engineer
Engineer::Engineer(int* Coal, int* Oxygen,std::vector<Generator>* GeneratorList, int* raw, int* ref):Colonist(Coal, Oxygen, GeneratorList)
{
  rawPtr = raw;
  refPtr = ref;
}
void Engineer::do_work()
{

}
//__Miner
Miner::Miner(int* Coal, int* Oxygen,std::vector<Generator>* GeneratorList, int* raw):Colonist(Coal, Oxygen, GeneratorList)
{
  rawPtr = raw;
}
void Miner::do_work()
{

}
//__Caretaker
Caretaker::Caretaker(int* Coal, int* Oxygen,std::vector<Generator>* GeneratorList, std::vector<Colonist*>* PeopleList):Colonist(Coal, Oxygen, GeneratorList)
{
  PatientList = PeopleList;
}
void Caretaker::do_work()
{

}
