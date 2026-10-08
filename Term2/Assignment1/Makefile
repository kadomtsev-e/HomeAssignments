BIN_DIR = bin
OBJ_DIR = obj
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic
SANITIZERS = -fsanitize=address,undefined -fno-omit-frame-pointer

EXTERN_SRC = src/extern_solution/main.cpp src/extern_solution/a.cpp src/extern_solution/b.cpp src/extern_solution/stats.cpp
INLINE_SRC = src/inline_solution/main.cpp src/inline_solution/a.cpp src/inline_solution/b.cpp

EXTERN_OBJ = $(patsubst src/extern_solution/%.cpp,$(OBJ_DIR)/extern/%.o,$(EXTERN_SRC))
INLINE_OBJ = $(patsubst src/inline_solution/%.cpp,$(OBJ_DIR)/inline/%.o,$(INLINE_SRC))

.PHONY: all test sanitize clean cleanall

all: $(BIN_DIR)/odr_extern $(BIN_DIR)/odr_inline

$(BIN_DIR)/odr_extern: $(EXTERN_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/odr_inline: $(INLINE_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/extern/%.o: src/extern_solution/%.cpp
	@mkdir -p $(OBJ_DIR)/extern
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/inline/%.o: src/inline_solution/%.cpp
	@mkdir -p $(OBJ_DIR)/inline
	$(CXX) $(CXXFLAGS) -c -o $@ $<

test: all
	@test "$$($(BIN_DIR)/odr_extern)" = "ok"
	@test "$$($(BIN_DIR)/odr_inline)" = "ok"
	@echo "All ODR tests passed."

sanitize: cleanall
	$(MAKE) CXXFLAGS="$(CXXFLAGS) $(SANITIZERS)" test

clean:
	rm -rf $(OBJ_DIR)

cleanall: clean
	rm -rf $(BIN_DIR)
