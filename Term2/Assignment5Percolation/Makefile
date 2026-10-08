PROJECT = percolation
TEST_PROJECT = test_percolation

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -Isrc
SANITIZERS = -fsanitize=address,undefined -fno-omit-frame-pointer
BIN_DIR = bin
OBJ_DIR = obj

SRC = src/main.cpp src/percolation.cpp
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
TEST_SRC = tests/test_percolation.cpp src/percolation.cpp
TEST_OBJ = $(TEST_SRC:%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all test sanitize clean cleanall

all: $(BIN_DIR)/$(PROJECT)

test: $(BIN_DIR)/$(TEST_PROJECT)
	./$(BIN_DIR)/$(TEST_PROJECT)

sanitize: cleanall
	$(MAKE) CXXFLAGS="$(CXXFLAGS) $(SANITIZERS)" test

$(BIN_DIR)/$(PROJECT): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/$(TEST_PROJECT): $(TEST_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR)

cleanall: clean
	rm -rf $(BIN_DIR)
