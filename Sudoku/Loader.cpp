#include "pch.h"
#include "Loader.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace Sudoku;

Puzzle Loader::load(std::string filename)
{
    std::ifstream stream(filename);
    if (!stream.is_open())
    {
        throw std::invalid_argument("file not found");
    }

    std::string name;
    stream >> name;

    unsigned int rows;
    unsigned int columns;
    stream >> rows;
    stream >> columns;

    auto puzzle = Loader::makePuzzle(name, rows, columns);

    stream.close();

    return puzzle;
}

Puzzle Sudoku::Loader::parseText(std::string text)
{
    auto ss = std::stringstream(text);

    std::string name;
    unsigned int rows, columns, digits;
    ss >> name >> rows >> columns >> digits;

    auto puzzle = Puzzle(rows, columns);

    std::string line;
    unsigned int row = 0;
    while (ss >> line)
    {
        for (auto column = 0u; column < columns; ++column)
        {
            auto ch = line[column];
            if (ch != '-')
            {
                puzzle.cell(row, column)->setClue(ch - '0');
            }
        }
        ++row;
    }

    return puzzle;
}

Puzzle Loader::makePuzzle(std::string name, unsigned int rows, unsigned int columns)
{
    if (name == "plain")
    {
        return Puzzle(rows, columns);
    }

    throw std::invalid_argument("invalid sudoku type");
}