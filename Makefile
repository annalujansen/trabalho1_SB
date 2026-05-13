CXX = g++-15
CXXFLAGS = -std=c++17 -Wall
SRC = src/main.cpp src/preprocessor.cpp src/assembler.cpp src/simulator.cpp
TARGET = montador

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) *.pre *.obj *.pen