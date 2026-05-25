#include "game_of_life.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

enum class ParseResult : std::uint8_t
{
    Ok,
    Help,
    Error,
};

static void printHelp(const std::string& progName)
{
    std::cout << "Usage: " << progName
              << " --input <path> [--iterations <number>] [--all] [--help]\n";
    std::cout << "\n";
    std::cout << "Options:\n";
    std::cout << "  --input <path>       Required input board file in _/* format\n";
    std::cout << "  --iterations <N>     Positive number of iterations (default: 1)\n";
    std::cout << "  --all                Write output for every iteration\n";
    std::cout << "  --help, -h           Show this help message\n";
    std::cout << "\n";
    std::cout << "Output files are written as inputname_iteration.extension\n";
}

struct Options
{
    std::string inputPath;
    int iterations = 1; // default positive value
    bool printAll = false;
};

ParseResult parseArgs(int argc, char* argv[], Options& opts, std::string& err)
{
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--input")
        {
            if (i + 1 >= argc)
            {
                err = "--input requires a path";
                return ParseResult::Error;
            }
            opts.inputPath = argv[++i];
        }
        else if (arg == "--iterations")
        {
            if (i + 1 >= argc)
            {
                err = "--iterations requires a number";
                return ParseResult::Error;
            }
            try
            {
                const std::string value = argv[++i];
                size_t parsedChars = 0;
                opts.iterations = std::stoi(value, &parsedChars);
                if (parsedChars != value.size())
                {
                    err = "invalid iterations";
                    return ParseResult::Error;
                }
            }
            catch (const std::exception&)
            {
                err = "invalid iterations";
                return ParseResult::Error;
            }
            if (opts.iterations <= 0)
            {
                err = "iterations must be positive";
                return ParseResult::Error;
            }
        }
        else if (arg == "--all")
        {
            opts.printAll = true;
        }
        else if (arg == "--help" || arg == "-h")
        {
            printHelp(argv[0]);
            return ParseResult::Help;
        }
        else
        {
            err = std::string("unknown argument '") + arg + "'";
            return ParseResult::Error;
        }
    }
    return ParseResult::Ok;
}

int main(int argc, char* argv[])
{
    Options opts;
    std::string err;
    const ParseResult parseResult = parseArgs(argc, argv, opts, err);
    if (parseResult == ParseResult::Help)
    {
        return 0;
    }
    if (parseResult == ParseResult::Error)
    {
        std::cerr << "Error: " << err << "\n";
        std::cerr << "Use --help for usage.\n";
        return 1;
    }

    if (opts.inputPath.empty())
    {
        std::cerr << "Error: --input is required\n";
        return 1;
    }

    try
    {
        Grid grid = loadGrid(opts.inputPath);

        for (int iter = 1; iter <= opts.iterations; ++iter)
        {
            grid = step(grid);
            if (opts.printAll)
            {
                std::string out = makeOutputPath(opts.inputPath, iter);
                writeGrid(grid, out);
                std::cout << "Saved result to " << out << "\n";
            }
        }

        if (!opts.printAll)
        {
            std::string out = makeOutputPath(opts.inputPath, opts.iterations);
            writeGrid(grid, out);
            std::cout << "Saved result to " << out << "\n";
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
