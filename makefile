CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: gatorTaxi

gatorTaxi: gatorTaxi.cpp
	$(CXX) $(CXXFLAGS) -o gatorTaxi gatorTaxi.cpp

clean:
	rm -f gatorTaxi
