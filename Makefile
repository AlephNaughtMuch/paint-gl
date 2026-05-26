# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -O2 -Ithird_party/glad/include -Iinclude

# Linker flags
LDFLAGS = -lglfw -lGL

# Target 
TARGET = paint-gl

# Source files
SRCS = src/*.cpp third_party/glad/glad.c 

# Rule to link src files into the target executable
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

# Default rule to build and run executable
all: $(TARGET)

# Rule to run the executable
run: $(TARGET)
	./$(TARGET)

# Clean rule to remove generated files
clean: 
	rm $(TARGET)