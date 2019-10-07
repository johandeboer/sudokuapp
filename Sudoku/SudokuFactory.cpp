#include "pch.h"
#include "SudokuFactory.h"
#include "Rule.h"
#include "Cell.h"
#include "RuleFactory.h"

using namespace Sudoku;

Puzzle SudokuFactory::plainSudoku()
{
    unsigned int k = 3;
    unsigned int n = k*k;
    auto puzzle = Puzzle(n, n);

    for (auto i = 0u; i < n; ++i)
    {
        // rows
        puzzle.addRule(RuleFactory::createHorizontalLine(puzzle, i, 0, n));
        // columns
        puzzle.addRule(RuleFactory::createVerticalLine(puzzle, 0, i, n));
    }

    for (auto i = 0u; i < n; i += k)
    for (auto j = 0u; j < n; j += k)
    {
        puzzle.addRule(RuleFactory::createBlock(puzzle, i, j, k));
    }

    return puzzle;
}
