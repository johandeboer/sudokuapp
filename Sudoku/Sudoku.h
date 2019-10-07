#pragma once

#include <vector>
#include "Cell.h"

namespace Sudoku
{

class Rule;

class Puzzle
{
public:
    using Grid = std::vector<Cell>;

    Puzzle(unsigned int columns, unsigned int rows);

    unsigned int columns() const { return mColumns; }
    unsigned int rows() const { return mRows; }
    Grid grid() const { return mGrid; }

    void addRule(const Sudoku::Rule & rule);

    Cell * cell(unsigned int index);
    Cell * cell(unsigned int row, unsigned int column);

private:
    unsigned int mColumns;
    unsigned int mRows;
    Grid mGrid;
    std::vector<Rule> mRules;
};

}