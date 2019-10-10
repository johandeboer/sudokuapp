#pragma once

#include "Rule.h"

namespace Sudoku
{

class RuleFactory
{
public:
    static void createHorizontalLine(const std::shared_ptr<Sudoku::Puzzle> & puzzle, unsigned int row, unsigned int column, unsigned int count);
    static void createVerticalLine(const std::shared_ptr<Sudoku::Puzzle> & puzzle, unsigned int row, unsigned int column, unsigned int count);
    static void createBlock(const std::shared_ptr<Sudoku::Puzzle> & puzzle, unsigned int row, unsigned int column, unsigned int size);
};

}