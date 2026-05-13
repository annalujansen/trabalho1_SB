CXX = g++-15
CXXFLAGS = -std=c++17 -Wall
SRC = src/main.cpp src/preprocessador.cpp src/montador.cpp src/simulador.cpp
TARGET = montador

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) *.pre *.obj *.pen