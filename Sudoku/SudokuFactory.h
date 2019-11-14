#pragma once

#include "Sudoku.h"

namespace Sudoku
{

class SudokuFactory
{
public:
    static std::shared_ptr<Puzzle> plainSudoku(ILogger * logger);
    static void makePlainRules(const std::shared_ptr<Puzzle> & puzzle);
    static void makeMixRules(const std::shared_ptr<Puzzle> & puzzle, std::stringstream * ss);
};

}