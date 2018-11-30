
#ifndef OXYGEN_H
#define OXYGEN_H
#include <gtkmm.h>
#include <iostream>
#include <string>
#include <vector>

#define STRESS_ON_WORK 20 // The amount of stress gained by working
#define ADD_COAL_AMOUNT 1
#define COLONISTS_TO_WIN 20


class Batteries
{
  int* Oxygen; // Accessed Variables
  int* RawPtr;
  int* RefPtr;
 public:
  int MaxPower;
  int Power;
  Batteries(int* Oxygen, int* Raw, int* Ref);
  void refine_metal(int amount); // Spend power and raw to make refined
  void hydrolysis(int amount); // Spend power to make oxygen. STRETCH GOAL - Make hydrogen for FUSION GENERATORS <3
};
class Generator
{
  int* OxygenPtr; // Generators use Oxygen
  int  efficiency;
  Batteries* PowerGrid; // Access to batteries in charge_battery;
 public:
  int coal;
  Generator(int* Oxygen);
  void charge_battery();
};

class Colonist
{
 protected:
  int stress;
  std::string name;
  int* CoalPtr;  // All colonists can change coal.
  int* OxygenPtr; // All colonists change Oxygen values
  std::vector<Generator*>* GenAccess;
  Generator* find_gen(); // Returns most "needy" generator i.e. an empty one from Generators.
 public:
  Colonist(std::string name, int* Coal, int* Oxygen, std::vector<Generator*>* GeneratorList);
  ~Colonist();
  void add_coal();
  virtual void do_work() = 0;
      
};
class Engineer:public Colonist
{
  int* rawPtr;
  int* refPtr;
 public:
  Engineer(std::string name, int* Coal, int* Oxygen,std::vector<Generator*>* GeneratorList, int* raw, int* ref);
  void do_work();
};
class Miner:public Colonist
{
  int* rawPtr;
 public:
  Miner(std::string name, int* Coal, int* Oxygen,std::vector<Generator*>* GeneratorList, int* raw);
  void do_work();
};
class Caretaker:public Colonist //
{
  std::vector<Colonist*>* PatientList;
  Colonist* find_most_stressed(); // Returns the most stressed colonist from PatientList.
  Colonist* Patient;
 public:
  Caretaker(std::string name, int* Coal, int* Oxygen,std::vector<Generator*>* GeneratorList, std::vector<Colonist*>* PeopleList);
  void do_work();
};

//_______________________________________________________________________________
class ColonyWindow:public Gtk::Window
{
public:
    ColonyWindow(std::vector<std::string> FileInput);
    virtual ~ColonyWindow();

    // Used to read in data from a file, every line is recorded in a vector.
    static std::vector<std::string> read_file(std::string filename);

protected:
    //____________ GUI STUFF ____________________
    void on_button_DayStart();

    Gtk::Label label_resources; // Displays Day, resource counts on Window.
    
    Gtk::Button button_DayStart;
    
    Gtk::Button quit;
    Gtk::Grid grid;

    std::vector<std::string> HighScores;
    //______________ OXYGEN STUFF _____________________
    int coal, oxygen, raw_metal, ref_metal, day;
    std::string player_name;
    std::vector<Colonist*> Colonists;
    std::vector<Generator> Generators;
    
    void end_game(); // Ends the game if # colonists = 20 (win) , or Oxygen < 0 (loss). Prints High Scores.
    void create_colonist(); // Should let you decide their job, and reduce stress of all colonists
    void remove_colonist(); // Should stress all colonists, and remove the target colonist.

 };

#endif 
