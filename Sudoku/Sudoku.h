#pragma once

#include <vector>
#include "Cell.h"
#include "Rule.h"

namespace Sudoku
{

class Rule;
class Cell;

class Puzzle
{
public:
    Puzzle(unsigned int columns, unsigned int rows, unsigned int digits);

    Puzzle(const Puzzle &) = delete;
    Puzzle & operator =(Puzzle const &) = delete;
    Puzzle(const Puzzle &&) = delete;
    Puzzle & operator =(Puzzle const &&) = delete;

    unsigned int columns() const { return mColumns; }
    unsigned int rows() const { return mRows; }
    unsigned int digits() const { return mDigits; }
    std::vector<Cell> grid() const { return mGrid; }

    void addRule(const Rule && rule);

    Cell * cell(unsigned int index);
    Cell * cell(unsigned int row, unsigned int column);

private:
    unsigned int mColumns;
    unsigned int mRows;
    unsigned int mDigits;
    std::vector<Cell> mGrid;
    std::vector<Rule> mRules;
};

}