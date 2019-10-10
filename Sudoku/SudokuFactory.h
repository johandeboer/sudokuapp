#pragma once

#include "Sudoku.h"

namespace Sudoku
{

class SudokuFactory
{
public:
    static Puzzle plainSudoku();
    static void makePlainRules(Puzzle & puzzle);
};

}