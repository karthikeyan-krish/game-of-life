#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <string>
#include <vector>

using Grid = std::vector<std::vector<bool>>;

Grid loadGrid(const std::string& path);
void writeGrid(const Grid& grid, const std::string& path);
Grid step(const Grid& grid);
std::string makeOutputPath(const std::string& inputPath, int iter);

#endif // GAME_OF_LIFE_H
