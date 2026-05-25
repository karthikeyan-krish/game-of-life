#include "game_of_life.h"
#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>
#include <string>

using namespace std;

static string writeTempFile(const string& contents)
{
    auto path = filesystem::temp_directory_path() / "game_of_life_test_input.txt";
    ofstream out(path);
    out << contents;
    return path.string();
}

TEST(LoadGridTest, ParsesValidBoard)
{
    string path = writeTempFile("_*_\n***\n");
    Grid grid = loadGrid(path);
    EXPECT_EQ(grid.size(), 2);
    EXPECT_EQ(grid[0].size(), 3);
    EXPECT_FALSE(grid[0][0]);
    EXPECT_TRUE(grid[0][1]);
    EXPECT_FALSE(grid[0][2]);
    EXPECT_TRUE(grid[1][0]);
    EXPECT_TRUE(grid[1][1]);
    EXPECT_TRUE(grid[1][2]);
}

TEST(LoadGridTest, ParsesEmptyFileAsEmptyGrid)
{
    string path = writeTempFile("");
    Grid grid = loadGrid(path);
    EXPECT_TRUE(grid.empty());
}

TEST(LoadGridTest, ThrowsOnInvalidCharacter)
{
    string path = writeTempFile("*x*\n");
    EXPECT_THROW(loadGrid(path), runtime_error);
}

TEST(LoadGridTest, ThrowsOnInconsistentLineLength)
{
    string path = writeTempFile("*_*\n**\n");
    EXPECT_THROW(loadGrid(path), runtime_error);
}

TEST(WriteGridTest, WritesExpectedFormat)
{
    Grid grid = {{true, false}, {false, true}};
    auto path = filesystem::temp_directory_path() / "game_of_life_test_output.txt";
    writeGrid(grid, path.string());
    ifstream input(path);
    string firstLine;
    string secondLine;
    getline(input, firstLine);
    getline(input, secondLine);
    EXPECT_EQ(firstLine, "*_");
    EXPECT_EQ(secondLine, "_*");
}

TEST(MakeOutputPathTest, AddsIterationBeforeExtension)
{
    EXPECT_EQ(makeOutputPath("game_of_life.txt", 5), "game_of_life_5.txt");
}

TEST(MakeOutputPathTest, HandlesFileWithoutExtension)
{
    EXPECT_EQ(makeOutputPath("game_of_life", 5), "game_of_life_5");
}

TEST(MakeOutputPathTest, KeepsDirectory)
{
    EXPECT_EQ(makeOutputPath("boards/game_of_life.txt", 5), "boards/game_of_life_5.txt");
}

TEST(CountNeighborsTest, CountsCorrectNeighbors)
{
    Grid grid = {{true, true, false}, {false, true, false}, {false, false, true}};
    EXPECT_EQ(countNeighbors(grid, {0, 0}), 2);
    EXPECT_EQ(countNeighbors(grid, {1, 1}), 3);
    EXPECT_EQ(countNeighbors(grid, {2, 2}), 1);
}

TEST(StepTest, ExpandsAndTrims)
{
    Grid grid = {{false, true, false}, {false, false, true}, {true, true, true}};
    Grid next = step(grid);
    Grid expected = {{true, false, true}, {false, true, true}, {false, true, false}};
    EXPECT_EQ(next, expected);
    EXPECT_EQ(next.size(), 3);
    EXPECT_EQ(next[0].size(), 3);
}

TEST(StepTest, ReturnsSmallestBoardContainingLivingCells)
{
    Grid grid = {{true, true, true}};
    Grid next = step(grid);
    Grid expected = {{true}, {true}, {true}};
    EXPECT_EQ(next, expected);
}

TEST(StepTest, ReturnsEmptyBoardWhenAllCellsDie)
{
    Grid grid = {{true}};
    Grid next = step(grid);
    EXPECT_TRUE(next.empty());
}
