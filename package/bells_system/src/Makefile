all: bells_system

bells_system: main.o config.o timer.o GPIOClass.o	
	$(CXX) main.o config.o timer.o GPIOClass.o states.cpp -lrt -o bells_system  

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

config.o: config.cpp
	$(CXX) $(CXXFLAGS) -c config.cpp

timer.o: timer.cpp
	$(CXX) $(CXXFLAGS) -c timer.cpp  

GPIOClass.o: GPIOClass.cpp
	$(CXX) $(CXXFLAGS) -c GPIOClass.cpp

states.o: states.cpp
	$(CXX) $(CXXFLAGS) -c states.cpp
clean:
	rm -rf *.o main
