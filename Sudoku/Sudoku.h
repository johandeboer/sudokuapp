#pragma once

#include <vector>
#include "Cell.h"
#include "Rule.h"

namespace Sudoku
{

class Puzzle
{
public:
    using Grid = std::vector<Cell>;

    Puzzle(int columns, int rows);

    int columns() const { return mColumns; }
    int rows() const { return mRows; }
    Grid grid() const { return mGrid; }

    void addRule(const Sudoku::Rule & rule);

    Cell * cell(unsigned int index);
    Cell * cell(unsigned int row, unsigned int column);

private:
    int mColumns;
    int mRows;
    Grid mGrid;
    std::vector<Rule> mRules;
};

}