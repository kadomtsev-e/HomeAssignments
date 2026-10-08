PROJECT = transformers
TEST_PROJECT = test_transformers

CXX = g++
CXXFLAGS = -Wall -Wextra -Isrc
LDFLAGS = 
GTEST_LIBS = -lgtest -lgtest_main -pthread

SRC = src/main.cpp src/weapon.cpp src/pilot.cpp src/transformer.cpp src/autobot.cpp src/decepticon.cpp src/minicon.cpp
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

TEST_SRC = tests/test_transformers.cpp src/weapon.cpp src/pilot.cpp src/transformer.cpp src/autobot.cpp src/decepticon.cpp src/minicon.cpp
TEST_OBJ = $(TEST_SRC:%.cpp=$(OBJ_DIR)/%.o)

BIN_DIR = bin
OBJ_DIR = obj

.PHONY: all clean cleanall test

all: $(BIN_DIR)/$(PROJECT)

test: $(BIN_DIR)/$(TEST_PROJECT)
	./$(BIN_DIR)/$(TEST_PROJECT)

$(BIN_DIR)/$(PROJECT): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(BIN_DIR)/$(TEST_PROJECT): $(TEST_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(GTEST_LIBS)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -rf $(OBJ_DIR)

cleanall:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
