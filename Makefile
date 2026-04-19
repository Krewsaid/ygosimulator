CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = ygosimulator
SRC_DIR = .
OBJ_DIR = build

SOURCES = main.cpp activity.cpp action.cpp search.cpp gamestate.cpp
OBJECTS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
