#include "pch.h"
#include "Loader.h"
#include "SudokuFactory.h"
#include <sstream>

using namespace Sudoku;

std::shared_ptr<Puzzle> Sudoku::Loader::parseText(std::string text)
{
    auto ss = std::stringstream(text);

    std::string name;
    unsigned int rows, columns, digits;
    ss >> name >> rows >> columns >> digits;

    auto puzzle = std::make_shared<Puzzle>(rows, columns, digits);
    SudokuFactory::makePlainRules(puzzle);

    std::string line;
    unsigned int row = 0;
    while (ss >> line)
    {
        for (auto column = 0u; column < columns; ++column)
        {
            auto ch = line[column];
            if (ch != '-')
            {
                puzzle->cell(row, column)->setClue(ch - '0');
            }
        }
        ++row;
    }

    return puzzle;
}
