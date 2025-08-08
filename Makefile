CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -Iinclude
SOURCES = src/BigInt.cpp src/main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = bigint_demo.exe

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q src\*.o $(TARGET) 2>nul || echo Cleaned

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run