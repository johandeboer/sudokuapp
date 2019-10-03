#pragma once

#include <vector>
#include "Cell.h"

namespace Sudoku
{

class Rule
{
public:
    Rule(std::vector<Cell *> cells);

private:
    std::vector<Cell *> mCells;
};

}