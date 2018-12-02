
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
    button_score.add_label("~Submit~");
    quit.add_label("~Quit~");
    label_scores.set_text("Sorted High Scores print here.");
    name_in.set_text("Better luck next time!");

    // Signal Handling.
    button_score.signal_clicked().connect(sigc::mem_fun(*this,&ColonyWindow::on_button_score));
    quit.signal_clicked().connect(sigc::mem_fun(*this,&ColonyWindow::close));

    // Widget Arrangement
    grid.attach(label_scores,   0,1,3,5);
    grid.attach(button_score,      2,8,1,1);
    grid.attach(quit,              2,9,1,1);
    grid.attach(name_in,           0,8,2,1);

    button_score.set_sensitive(FALSE); // Enabled in end_game(), if victory is acheived.
    name_in.set_sensitive(FALSE);

    grid.show_all();
    add(grid);

    // Startup Dialog
    Gtk::MessageDialog dialog(*this, "You are in charge of leading a small space colony!\n", false, Gtk::MESSAGE_INFO);
    dialog.set_secondary_text("To survive, you must produce power and create enough oxygen to breathe and grow, good luck Boss!\n\nRead the README for a tutorial on mechanics!");
    dialog.run();
    dialog.close();
    //____________________OXYGEN STUFF___________________
    coal = 2;
    oxygen = 100;
    raw_metal = 0;
    ref_metal = 0;
    day = 1;
    Batteries BB(&oxygen, &raw_metal, &ref_metal);
    Generator FirstGenerator(&oxygen, &BB, 1);
    Generators.push_back(FirstGenerator);

    create_colonist(); // First Colonist.
    // Begin the game running thing.
    int i = 1;
    while(i)
      {
	if(end_game())
	  i = 0;
	else
	  {// Try to end the game upon day start

	    //Ask if a new colonist is wanted
	    if(day>1)
	      {
		Gtk::MessageDialog prompt_for_colonist(*this, "Do you want to call a new colonist today?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
		prompt_for_colonist.set_secondary_text("It is currently day "+std::to_string(day)+"\nOxygen = "+std::to_string(oxygen)+"\nCoal = "+std::to_string(coal)+"\nRaw Metal= "+std::to_string(raw_metal)+"\nRefined Metal = "+std::to_string(ref_metal)+"\nColonists = "+std::to_string(Colonists.size())+"\nGenerators = "+std::to_string(Generators.size())+"\nCurrentPower / Power capacity = "+std::to_string(BB.Power)+"/"+std::to_string(BB.MaxPower));

		if(prompt_for_colonist.run() == Gtk::RESPONSE_YES)
		  {
		    create_colonist();
		    stress_all_colonists(-5);
		  }
	      }
	    // Remove stressed colonists
	    for(int j=0;j<Colonists.size();j++)
	      {
		if(Colonists[j]->stress >= 100)
		  {
		    std::cout<<"Colonist "<<Colonists[j]->name<<" left due to stress"<<std::endl;
		    Colonists.erase(Colonists.begin()+j);
		    stress_all_colonists(5);
		    j = 0; // Colonists stresses changed, so have to check everyone again.
		  }
	      }
	    // Let the user do a GUI thing to assign job assignments and submit.

	    //
	    for(int j = 0; j<Colonists.size();j++)
	      {
		// Colonists preform assignment.
		switch(Colonists[j]->assignment)
		  {
		  case 0:
		    Colonists[j]->rest();
		    break;
		  case 1:
		    Colonists[j]->do_work();
		    break;
		  case 2:
		    Colonists[j]->add_coal();  // New colonists do_work by default. Can be changed in the Colonist base class constructor for test purposes.
		    break;
		  }
	      }
	    for(int j=0; j<Generators.size();j++)
	      {
		// Generators burn their coal.
		Generators[j].charge_batteries();
	      }
	    // Let the user do a GUI that prompts the user for the amount of power to spend on a task.
	    //Batteries spend power
	    BB.hydrolysis(5); // Temporary, we just have 5 power going to Oxygen.

	    oxygen -= Colonists.size() * COLONISTS_DAILY_OXYGEN;
	    day++;
	  }
      }
    sort_scores();
}
ColonyWindow::~ColonyWindow() // Tommy
{

}
int ColonyWindow::end_game() // Ends the game if a loss or win condition is met. // Tommy
{
  if(oxygen < 0) // Loss
    {
      Gtk::MessageDialog dialog(*this, "Game Over: Lost Colony\n", false, Gtk::MESSAGE_INFO);
      dialog.set_secondary_text("Your oxygen was depleted. Remember that burning generators and additional people consume oxygen.");
      dialog.run();
      return 1;
    }
  else if(Colonists.size() == 0) // Loss
    {
      Gtk::MessageDialog dialog(*this, "Game Over: Abandoned Colony\n", false, Gtk::MESSAGE_INFO);
      dialog.set_secondary_text("Your colonists all left. Remember that stressed colonists become unproductive and need a rest.");
      dialog.run();
      return 1;
    }
  if(Colonists.size() == COLONISTS_TO_WIN) // Win
    {
      Gtk::MessageDialog dialog(*this, "INDEPENDENCE: Victory!\n", false, Gtk::MESSAGE_INFO);
      dialog.set_secondary_text("Your colony is now able to run without your help, well done!\nDays to independence: "+std::to_string(day));
      dialog.run();
      name_in.set_text("Name");
      button_score.set_sensitive(); // Enabled in end_game(), if victory is acheived.
      name_in.set_sensitive();
      return 1;
    }
  return 0;
}
void ColonyWindow::create_colonist() // Tommy
{
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
    {
      NewGuy->name = entry->get_text();
      Colonists.push_back(NewGuy);
      std::cout<<"Colonist added: "<<Colonists[Colonists.size()-1]->name<<std::endl;
    }
  dialog->close();
  delete dialog;
  delete label;
  delete entry;
}
void ColonyWindow::stress_all_colonists(int amount) // Tommy
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
void ColonyWindow::sort_scores() // Sorts scores and updates label.
{
  std::vector<std::string> temp;
  for(int i = 0; i<HighScores.size();i += 2)
    {
      temp.push_back("Days : "+HighScores[i+1]+" ---------- "+HighScores[i]);
    }
  std::sort(temp.begin(), temp.end());
  std::string scorestext = "~~~~HIGH SCORES~~~~";
  for(int i=0;i<temp.size()&& i<20;i++) // Prints all records up to a max of 20.
    {
      scorestext = scorestext+ "\n" +temp[i];
    }
  label_scores.set_text(scorestext);
}
//Button Functionality.
void ColonyWindow::on_button_score() // Should allow the user to submit their name
{
  // adds name
  std::string addition = name_in.get_text();
  HighScores.push_back(addition);
  // sorts list again
  sort_scores();

  name_in.set_sensitive(FALSE); // Disables these elements so the user doesnt enter their name multiple times.
  button_score.set_sensitive(FALSE);
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
void Batteries::refine_metal(int amount) // Amount is the amount of power to spend. //Michael // Tommy
{
  int ref_possible = amount / POWER_PER_REF; // So, if Amount = 17 but a ref takes 5 power to make, use up to 15 power.
  for(int i = 0; i<ref_possible && Power > POWER_PER_REF && *RawPtr > RAW_PER_REF; i++)
    {
      Power -= POWER_PER_REF;
      *RefPtr += 1;
      *RawPtr -= RAW_PER_REF;
    }
}
void Batteries::hydrolysis(int amount) // Amount is the amount of power to spend. //Michael // Tommy
{
  for(int i = 0; i<amount && Power > 0; i++)
    {
      Power--;
      *OxygenPtr += OXYGEN_PER_POWER;
    }
}
//___________________Generator Class Implementation________________
Generator::Generator(int* oxygen, Batteries* PowerOut, int efficiency) // Tommy
{
  OxygenPtr = oxygen;
  this->efficiency = efficiency;
  PowerGrid = PowerOut;

  internal_coal = 0;
}
void Generator::charge_batteries() // Tommy
{
  if(internal_coal>0)
    {
      for(int i = 0; i<COAL_PER_BURN && internal_coal != 0; i++)
	{
	  internal_coal--;
	  *OxygenPtr -= OXYGEN_PER_BURN;
	  PowerGrid->Power += BASE_EFF_OUTPUT * efficiency;
	  std::cout<<"Coal burnt."<<std::endl;
	}
      if(PowerGrid->Power > PowerGrid->MaxPower)
	PowerGrid->Power = PowerGrid->MaxPower;
    }
}


//____________________Colonist Class Implementation_______________
Colonist::Colonist(int* Coal, int* Oxygen, std::vector<Generator>* GeneratorList) // Tommy
{
  CoalPtr = Coal;
  OxygenPtr = Oxygen;
  stress = 0;
  GenAccess = GeneratorList;
  assignment = 2;
}
Colonist::~Colonist() // Tommy
{

}
Generator* Colonist::find_gen() // Returns the most empty generator. // Tommy
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
  if(stress < STRESS_THRESHOLD)
    {
      Generator* targetGenerator = find_gen();
      int i;
      for(i = 0; i<ADD_COAL_AMOUNT && *CoalPtr != 0; i++)
	{
	  *CoalPtr -= 1;
	  targetGenerator->internal_coal++;
	}
      std::cout<<name<<" - Shovled "<<i<<" coal"<<std::endl;
    }
  else
    {
      std::cout<<name<<" was too stressed to work."<<std::endl;
    }
  stress += STRESS_ON_WORK;
}
void Colonist::rest() // Tommy
{
  std::cout<<name<<" - Taking day off"<<std::endl;
  stress -= COLONIST_REST_AMOUNT;
  if (stress < 0)
    stress = 0;
}

//__Engineer
Engineer::Engineer(int* Coal, int* Oxygen,std::vector<Generator>* GeneratorList, int* raw, int* ref):Colonist(Coal, Oxygen, GeneratorList)
{
  rawPtr = raw;
  refPtr = ref;
}
void Engineer::do_work()
{
  std::cout<<"Engineer "<<name<<"- Working!"<<std::endl;
}
//__Miner
Miner::Miner(int* Coal, int* Oxygen,std::vector<Generator>* GeneratorList, int* raw):Colonist(Coal, Oxygen, GeneratorList)
{
  rawPtr = raw;
}
void Miner::do_work()
{
  std::cout<<"Miner "<<name<<"- Working!"<<std::endl;
}
//__Caretaker
Caretaker::Caretaker(int* Coal, int* Oxygen,std::vector<Generator>* GeneratorList, std::vector<Colonist*>* PeopleList):Colonist(Coal, Oxygen, GeneratorList)
{
  PatientList = PeopleList;
}
Colonist* Caretaker::find_most_stressed()//Andrea 
{
  Patient=&(*PatientList)[0];
  int highest_stress;
  for(int i=1,highest_stress=Patient->stress;i<PatientList->size();i++)
  {
    highest_stress=(*PatientList)[0].stress;
    Patient=&(*PatientList)[i];
  }
  return Patient;
}
void Caretaker::do_work()
{
  std::cout<<"Caretaker "<<name<<"- Working!"<<std::endl;
}
