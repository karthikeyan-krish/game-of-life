CMAKE_BUILD_DIR := build
EXECUTABLE := $(CMAKE_BUILD_DIR)/game-of-life
INPUT ?= game_of_life.txt
ITERATIONS ?= 5
ARGS ?=
CLANG_FORMAT ?= clang-format
CLANG_TIDY ?= clang-tidy

.PHONY: all build demo run format format-check tidy test ci clean

all: build

build:
	cmake -S . -B $(CMAKE_BUILD_DIR)
	cmake --build $(CMAKE_BUILD_DIR)

run: build
	$(EXECUTABLE) --input $(INPUT) --iterations $(ITERATIONS) $(ARGS)

# Run a demo with the sample input file and write every generated board.
demo: build
	$(EXECUTABLE) --input game_of_life.txt --iterations 5 --all

format:
	$(CLANG_FORMAT) -i src/*.cpp src/*.h tests/*.cpp

format-check:
	$(CLANG_FORMAT) --dry-run --Werror src/*.cpp src/*.h tests/*.cpp

tidy:
	cmake -S . -B $(CMAKE_BUILD_DIR)
	$(CLANG_TIDY) --quiet -p $(CMAKE_BUILD_DIR) src/*.cpp tests/*.cpp

test: build
	cd $(CMAKE_BUILD_DIR) && ctest --output-on-failure

check: format-check tidy build test
	@echo "Local checks passed"

ci: check
	@echo "CI checks passed"

clean:
	rm -rf $(CMAKE_BUILD_DIR)
