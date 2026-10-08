BIN_DIR = bin
OBJ_DIR = obj
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Isrc

EXTERN_SRC = src/extern_solution/main.cpp src/extern_solution/a.cpp src/extern_solution/b.cpp src/extern_solution/stats.cpp
INLINE_SRC = src/inline_solution/main.cpp src/inline_solution/a.cpp src/inline_solution/b.cpp

EXTERN_OBJ = $(patsubst src/extern_solution/%.cpp,$(OBJ_DIR)/extern/%.o,$(EXTERN_SRC))
INLINE_OBJ = $(patsubst src/inline_solution/%.cpp,$(OBJ_DIR)/inline/%.o,$(INLINE_SRC))

.PHONY: all clean

all: $(BIN_DIR)/odr_extern $(BIN_DIR)/odr_inline

$(BIN_DIR)/odr_extern: $(EXTERN_OBJ)
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/odr_inline: $(INLINE_OBJ)
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/extern/%.o: src/extern_solution/%.cpp
	@if not exist "$(OBJ_DIR)\\extern" mkdir "$(OBJ_DIR)\\extern"
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/inline/%.o: src/inline_solution/%.cpp
	@if not exist "$(OBJ_DIR)\\inline" mkdir "$(OBJ_DIR)\\inline"
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@if exist "$(OBJ_DIR)" rmdir /S /Q "$(OBJ_DIR)"
	@if exist "$(BIN_DIR)" rmdir /S /Q "$(BIN_DIR)"
