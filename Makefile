TARGET = backlight_control
SRC = main.cpp
CXX = g++
CXXFLAGS = -Wall
LDFLAGS = -lwiringPi

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
