# Compiler and flags
CXX = g++
CXXFLAGS = -I./Eigen -Wall 

# Target executable
TARGET = main

# Source files and object files
SRCS = main.cpp Hamming.cpp HammingDecode.cpp HammingEncode.cpp HammingErrorEncode.cpp
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)
	
# Phony targets
.PHONY: all clean
