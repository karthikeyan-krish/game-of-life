#include "game_of_life.h"

#include <filesystem>
#include <fstream>
#include <optional>
#include <stdexcept>

namespace
{
struct Bounds
{
    int top;
    int bottom;
    int left;
    int right;
};

Grid expandGrid(const Grid& grid)
{
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    Grid expanded(rows + 2, std::vector<bool>(cols + 2, false));

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            expanded[r + 1][c + 1] = grid[r][c];
        }
    }

    return expanded;
}

Grid applyRules(const Grid& expanded)
{
    const int rows = static_cast<int>(expanded.size());
    const int cols = static_cast<int>(expanded[0].size());
    Grid next(rows, std::vector<bool>(cols, false));

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            const int alive = countNeighbors(expanded, {r, c});
            next[r][c] = expanded[r][c] ? (alive == 2 || alive == 3) : (alive == 3);
        }
    }

    return next;
}

std::optional<Bounds> findLiveBounds(const Grid& grid)
{
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    Bounds bounds{rows, -1, cols, -1};

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            if (grid[r][c])
            {
                if (r < bounds.top)
                {
                    bounds.top = r;
                }
                if (r > bounds.bottom)
                {
                    bounds.bottom = r;
                }
                if (c < bounds.left)
                {
                    bounds.left = c;
                }
                if (c > bounds.right)
                {
                    bounds.right = c;
                }
            }
        }
    }

    if (bounds.bottom < 0)
    {
        return std::nullopt;
    }
    return bounds;
}

Grid trimGrid(const Grid& grid, const Bounds& bounds)
{
    Grid trimmed(bounds.bottom - bounds.top + 1,
                 std::vector<bool>(bounds.right - bounds.left + 1, false));

    for (int r = bounds.top; r <= bounds.bottom; ++r)
    {
        for (int c = bounds.left; c <= bounds.right; ++c)
        {
            trimmed[r - bounds.top][c - bounds.left] = grid[r][c];
        }
    }

    return trimmed;
}
} // namespace

Grid loadGrid(const std::string& path)
{
    std::ifstream input(path);
    if (!input)
    {
        throw std::runtime_error("cannot open input file '" + path + "'");
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line))
    {
        for (char cell : line)
        {
            if (cell != '_' && cell != '*')
            {
                throw std::runtime_error("invalid character in input file");
            }
        }
        lines.push_back(line);
    }

    if (lines.empty())
    {
        return {};
    }

    const size_t cols = lines[0].size();
    for (const auto& currentLine : lines)
    {
        if (currentLine.size() != cols)
        {
            throw std::runtime_error("inconsistent line lengths in input file");
        }
    }

    Grid grid(lines.size(), std::vector<bool>(cols, false));
    for (size_t r = 0; r < lines.size(); ++r)
    {
        for (size_t c = 0; c < cols; ++c)
        {
            grid[r][c] = (lines[r][c] == '*');
        }
    }
    return grid;
}

void writeGrid(const Grid& grid, const std::string& path)
{
    std::ofstream out(path);
    if (!out)
    {
        throw std::runtime_error("cannot write to output file '" + path + "'");
    }
    for (const auto& row : grid)
    {
        for (bool cell : row)
        {
            out << (cell ? '*' : '_');
        }
        out << '\n';
    }
}

int countNeighbors(const Grid& grid, CellPosition position)
{
    if (grid.empty() || grid[0].empty())
    {
        return 0;
    }

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    int count = 0;
    for (int dr = -1; dr <= 1; ++dr)
    {
        for (int dc = -1; dc <= 1; ++dc)
        {
            if (dr == 0 && dc == 0)
            {
                continue;
            }
            const int neighborRow = position.row + dr;
            const int neighborCol = position.col + dc;
            if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < cols &&
                grid[neighborRow][neighborCol])
            {
                ++count;
            }
        }
    }
    return count;
}

Grid step(const Grid& grid)
{
    if (grid.empty())
    {
        return grid;
    }

    // Add one dead border so cells can be born just outside the current edges
    // After applying the rules, keep only the smallest area containing live cells
    const Grid expanded = expandGrid(grid);
    const Grid next = applyRules(expanded);
    const std::optional<Bounds> bounds = findLiveBounds(next);
    if (!bounds)
    {
        return {};
    }

    return trimGrid(next, *bounds);
}

std::string makeOutputPath(const std::string& inputPath, int iter)
{
    size_t slash = inputPath.find_last_of("/\\");
    std::string dir = (slash == std::string::npos) ? std::string() : inputPath.substr(0, slash + 1);
    std::string file = (slash == std::string::npos) ? inputPath : inputPath.substr(slash + 1);
    size_t dot = file.find_last_of('.');
    std::string name = (dot == std::string::npos) ? file : file.substr(0, dot);
    std::string ext = (dot == std::string::npos) ? std::string() : file.substr(dot);
    return dir + name + "_" + std::to_string(iter) + ext;
}
