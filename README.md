# Game of Life

Small C++17 implementation of Conway's Game of Life.

The program reads a board from a text file, applies the rules for a positive
number of iterations, and writes the resulting board using the smallest grid
that still contains all living cells.

## Requirements

- CMake 3.10 or newer
- A C++17 compiler
- `make`
- `clang-format` and `clang-tidy` for local checks

GoogleTest is downloaded by CMake during configuration.

## Build

```bash
make
```

This configures CMake in `build/` and builds the executable:

```text
build/game-of-life
```

## Run

```bash
make run INPUT=game_of_life.txt ITERATIONS=5
```

Optional flags can be passed through `ARGS`:

```bash
make run INPUT=game_of_life.txt ITERATIONS=5 ARGS="--all"
```

You can also run the executable directly:

```bash
build/game-of-life --input game_of_life.txt --iterations 5
build/game-of-life --input game_of_life.txt --iterations 5 --all
```

`--iterations` must be positive. If omitted, the program uses the default value
`1`.

## Output

For an input file named:

```text
game_of_life.txt
```

iteration `5` is written to:

```text
game_of_life_5.txt
```

If `--all` is used, the program writes every computed iteration:

```text
game_of_life_1.txt
game_of_life_2.txt
...
game_of_life_5.txt
```

## Board Format

Input and output files use the same format:

- `_` is a dead cell
- `*` is a living cell
- all non-empty lines must have the same length
- no other characters are allowed
- an empty file is valid

Example:

```text
____*_*_*____
______*______
_____________
```

## Checks

Run the local CI target:

```bash
make ci
```

This runs:

- `clang-format` check
- `clang-tidy`
- build
- tests

Individual targets are also available:

```bash
make format
make tidy
make test
make clean
```

## Demo

```bash
make demo
```

This runs `game_of_life.txt` for 5 iterations and writes all generated boards.
