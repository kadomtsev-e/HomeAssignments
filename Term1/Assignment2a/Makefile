PROJECT = reverse
CXX = g++
CXXFLAGS = -Wall -Wextra -Isrc
SRC = src/main.cpp src/transform.cpp
OBJ = $(SRC:src/%.cpp=obj/%.o)
BIN_DIR = bin
OBJ_DIR = obj

.PHONY: all clean cleanall

all: $(BIN_DIR)/$(PROJECT)

$(BIN_DIR)/$(PROJECT): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -rf $(OBJ_DIR)

cleanall:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
