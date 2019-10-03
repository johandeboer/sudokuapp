#pragma once

#include <vector>
#include "Cell.h"

namespace Sudoku
{

class Sudoku
{
public:
    Sudoku(int columns, int rows);

private:
    int mColumns;
    int mRows;
    std::vector<Cell> grid;
};

}