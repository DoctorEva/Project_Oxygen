
#ifndef OXYGEN_H
#define OXYGEN_H
#include <gtkmm.h>
#include <iostream>
#include <string>
#include <vector>

#define COLONISTS_TO_WIN 20 // Win condition.
#define STRESS_THRESHOLD 70 // Past this stress, a colonist will not do work.
// Colonist action definitions.
#define COLONISTS_DAILY_OXYGEN 5
#define STRESS_ON_WORK 20 // The amount of stress gained by working
#define ADD_COAL_AMOUNT 1 // Amount of coal added during add_coal();
#define COLONIST_REST_AMOUNT 15 // The amount of stress releived by taking the day off.
// Generator actions definitions
#define BASE_EFF_OUTPUT 5 // Power output of a low eff. generator burning 1 coal.
#define CAPACITY_PER_BATTERY 10 // How much power each battery holds.
#define COAL_PER_BURN 2// How much coal is burnt in burning coal at max.
#define OXYGEN_PER_BURN 5// How much O2 is burnt per burning coal.
// Battery action definitions.
#define OXYGEN_PER_POWER 2// How much O2 is given per power spent.
#define POWER_PER_REF 5 // How much power does it take to make 1 refined metal.
#define RAW_PER_REF 2 // How much raw metal makes 1 refined metal.
// Caretaker
#define CARETAKER_EFFECT 80 // How much stress caretakers reduce.
// Miner
#define MINER_RAW 1 // How much raw is found per mining job.
#define MINER_COAL 2 // How much coal is found per mining job.

class Batteries
{
  int* OxygenPtr; // Accessed Variables
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
  int  efficiency; // Multiplier of base generator output.
  Batteries* PowerGrid; // Access to batteries in charge_battery;
 public:
  int internal_coal;
  Generator(int* Oxygen, Batteries* PowerOut, int efficiency);
  void charge_batteries();
};

class Colonist
{
 protected:
  int* CoalPtr;  // All colonists can change coal.
  int* OxygenPtr; // All colonists change Oxygen values
  std::vector<Generator>* GenAccess;
  Generator* find_gen(); // Returns most "needy" generator i.e. an empty one from Generators.
 public:
  int assignment;
  std::string name;
  int stress;
  Colonist(int* Coal, int* Oxygen, std::vector<Generator>* GeneratorList);
  ~Colonist();
  void add_coal();
  virtual void do_work() = 0;
  void rest();
  
};
class Engineer:public Colonist
{
  int* rawPtr;
  int* refPtr;
 public:
  Engineer(int* Coal, int* Oxygen,std::vector<Generator>* GeneratorList, int* raw, int* ref);
  void do_work();
};
class Miner:public Colonist
{
  int* rawPtr;
 public:
  Miner(int* Coal, int* Oxygen,std::vector<Generator>* GeneratorList, int* raw);
  void do_work();
};
class Caretaker:public Colonist //
{
  std::vector<Colonist*>* PatientList;
  Colonist* find_most_stressed(); // Returns the most stressed colonist from PatientList.
  Colonist* Patient;
 public:
  Caretaker(int* Coal, int* Oxygen,std::vector<Generator>* GeneratorList, std::vector<Colonist*>* PeopleList);
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
    static void output_file(std::vector<std::string> data);

protected:
    //____________ GUI STUFF ____________________
    void on_button_score();
    virtual void on_change_assignment(Gtk::Button* button, int i);
    Gtk::Button button_score;
    Gtk::Label label_scores; // Displays Day, resource counts on Window.
    Gtk::Entry name_in;
    
    Gtk::Button quit;
    Gtk::Grid grid;

    std::vector<std::string> HighScores;
    void sort_scores();
    //______________ OXYGEN STUFF _____________________
    int coal, oxygen, raw_metal, ref_metal, day;
    std::string player_name;
    std::vector<Colonist*> Colonists;
    std::vector<Generator> Generators;
    
    int end_game(); // Returns true if an end condition is met.
    void create_colonist(); // Should let you decide their job, and reduce stress of all colonists
    void stress_all_colonists(int amount); // Stresses all colonists. Can also destress.
    void start_change_assignment_dialog();
    void start_power_spend_dialog(Batteries* Battery);


 };

#endif 
