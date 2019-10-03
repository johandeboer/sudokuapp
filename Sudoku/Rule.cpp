#include "pch.h"
#include "Rule.h"

using namespace Sudoku;

Rule::Rule(std::vector<Cell *> cells) :
    mCells(std::move(cells))
{
}