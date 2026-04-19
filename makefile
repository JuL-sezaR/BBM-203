# Compiler and flags
CXX = g++
CXXFLAGS = -g -std=c++11 -Wall

# Directories (quoted to handle spaces)
OBJ_DIR = "obj"
BIN_DIR = "bin"

# Executable name
TARGET = $(BIN_DIR)/HUTrain

# Source and object files
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Ensure directories exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile source files into object files
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link all objects into final executable (only one target rule)
$(TARGET): $(BIN_DIR) $(OBJ_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) -o "$(TARGET)" $(OBJS)

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program with correct input file path (assuming input.txt exists in project root)
run: all
	"./$(TARGET)" "input.txt"

# Run with Valgrind
mem: all
	valgrind --leak-check=full --track-origins=yes "./$(TARGET)" "input.txt"

.PHONY: all clean run mem
