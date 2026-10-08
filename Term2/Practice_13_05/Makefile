PROJECT = office_tour
TEST_PROJECT = test_tour

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -Isrc
SANITIZERS = -fsanitize=address,undefined -fno-omit-frame-pointer
BIN_DIR = bin
OBJ_DIR = obj

COMMON_SRC = src/room.cpp src/guide.cpp src/tour.cpp
SRC = src/main.cpp $(COMMON_SRC)
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
TEST_SRC = tests/test_tour.cpp $(COMMON_SRC)
TEST_OBJ = $(TEST_SRC:%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all test sanitize clean cleanall

all: $(BIN_DIR)/$(PROJECT)

test: $(BIN_DIR)/$(TEST_PROJECT) $(BIN_DIR)/$(PROJECT)
	./$(BIN_DIR)/$(TEST_PROJECT)
	@printf '2\n3\n4\n5\n' | ./$(BIN_DIR)/$(PROJECT) > /dev/null

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
