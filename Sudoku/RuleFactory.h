#pragma once

#include "Rule.h"

namespace Sudoku
{

class RuleFactory
{
public:
    static void createHorizontalLine(const std::shared_ptr<Sudoku::Puzzle> & puzzle, size_t row, size_t column, unsigned int count);
    static void createVerticalLine(const std::shared_ptr<Sudoku::Puzzle> & puzzle, size_t row, size_t column, unsigned int count);
    static void createDiagonalLine(const std::shared_ptr<Puzzle> & puzzle, size_t index, unsigned int count, int stride);
    static void createBlock(const std::shared_ptr<Sudoku::Puzzle> & puzzle, size_t row, size_t column, unsigned int size);
};

}