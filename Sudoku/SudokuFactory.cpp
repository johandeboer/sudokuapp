#include "pch.h"
#include "SudokuFactory.h"
#include "Rule.h"
#include "Cell.h"

using namespace Sudoku;

Puzzle SudokuFactory::plainSudoku()
{
    unsigned int k = 3;
    unsigned int n = k*k;
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

    return puzzle;
}
