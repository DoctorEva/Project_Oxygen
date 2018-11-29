CXXFLAGS += -std=c++11
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`
ex: oxygen_main.o oxygen.o
	$(CXX) $(CXXFLAGS) -o Oxygen oxygen_main.o oxygen.o $(GTKFLAGS)
oxygen_main.o: oxygen_main.cpp oxygen.h
	$(CXX) $(CXXFLAGS) -c oxygen_main.cpp $(GTKFLAGS)
oxygen.o: oxygen.h oxygen.cpp
	$(CXX) $(CXXFLAGS) -c oxygen.cpp $(GTKFLAGS)
