#pragma once

#include "Rule.h"

namespace Sudoku
{

class RuleFactory
{
public:
    static void createHorizontalLine(Sudoku::Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int count);
    static void createVerticalLine(Sudoku::Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int count);
    static void createBlock(Sudoku::Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int size);
};

}