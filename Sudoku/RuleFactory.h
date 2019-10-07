#pragma once

#include "Rule.h"

namespace Sudoku
{

class RuleFactory
{
public:
    static HorizontalRule createHorizontalLine(Sudoku::Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int count);
    static VerticalRule createVerticalLine(Sudoku::Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int count);
    static BlockRule createBlock(Sudoku::Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int size);

private:
    static Rule createLine(Sudoku::Puzzle & puzzle, unsigned int startIndex, unsigned int count, unsigned int stride);
};

}