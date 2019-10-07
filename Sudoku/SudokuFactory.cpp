#include "pch.h"
#include "SudokuFactory.h"

using namespace Sudoku;

Puzzle SudokuFactory::plainSudoku()
{
    auto k = 3;
    auto n = k*k;
    auto puzzle = Puzzle(n, n);

    for (auto i = 0u; i < n; ++i)
    {
        // rows
        puzzle.addRule(Rule::createLine(puzzle, i * n, n, 1));
        // columns
        puzzle.addRule(Rule::createLine(puzzle, i, n, n));
    }

    for (auto i = 0u; i < n; i += k)
    for (auto j = 0u; j < n; j += k)
    {
        puzzle.addRule(Rule::createBlock(puzzle, i, j, k));
    }
}
