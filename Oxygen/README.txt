README
Project Oxygen: 
Thomas Tran
Andrea Munoz
Diego Vester
Micheal Ly
____________________________________
Files Included: 	Makefile
			oxygen.h
			oxygen.cpp
			oxygen_main.cpp
			High Scores List
_____________________________________
Welcome to Oxygen, 
your goal is to produce enough oxygen to grow your colony!
TUTORIAL:
	To win, grow your colony to 20 colonists, and don't run out of Oxygen.
	You start with one colonist, one generator, and one battery.

	To make oxygen, you will produce power and spend it on oxygen production.
	To make power, you will need to put coal into generators.
	To get coal, you need to mine with Miners, which also gives metal. Coal must then be shoveled into generators.

	Colonists can be engineers, who will let you use :
	Metal to build new batteries and generators. Metal can also be refined using power into refined metal.
	Refined metal to build the most efficient generators, which use less oxygen and coal per power.

	Things to watch out for!:
	Generators consume oxygen when burning coal. Colonists will constantly breathe.
	Colonists will gain stress whenever they work. At moderate stress, they won't be get anything done at work but will gain stress trying.
	At extreme stress, your colonist will give up, and leave the colony.
	Manage your stress with Caretakers, who will automatically reduce the stress of the most stressed colonist. They'll need destressing too, and can't care for themselves!
	Colonists will be said and gain a bit of stress when someone leaves. They will also reduce stress when welcoming a new friend!
	
_____________________________________

To compile:
In Ubuntu terminal:

	make
	./Oxygen