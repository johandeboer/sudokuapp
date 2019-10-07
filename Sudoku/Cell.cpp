#include "pch.h"
#include "Cell.h"

using namespace Sudoku;

Cell::Cell() : Cell(9)
{
}

Cell::Cell(int noDigits) :
    mDigits(noDigits),
    mSqrt(static_cast<int>(std::ceil(std::sqrt(noDigits))))
{
}
