#pragma once

#include <vector>
#include "Sudoku.h"

namespace Sudoku
{

class Rule
{
public:
    Rule();
    static Rule createLine(const Sudoku::Puzzle & puzzle, unsigned int startIndex, unsigned int count, unsigned int stride);
    static Rule createBlock(const Sudoku::Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int size);

protected:

    void setCells(std::vector<Cell *> cells);
    void addCell(Cell * cell);

private:
    std::vector<Cell *> mCells;
};

}