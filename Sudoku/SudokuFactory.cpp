#include "pch.h"
#include "SudokuFactory.h"
#include "Rule.h"
#include "Cell.h"
#include "RuleFactory.h"

using namespace Sudoku;

std::shared_ptr<Puzzle> SudokuFactory::plainSudoku()
{
    unsigned int k = 3;
    unsigned int n = k * k;
    auto puzzle = std::make_shared<Puzzle>(n, n, n);
    makePlainRules(puzzle);
    return puzzle;
}

void SudokuFactory::makePlainRules(const std::shared_ptr<Puzzle> & puzzle)
{
    auto n = puzzle->rows();
    auto k = static_cast<unsigned int>(sqrt(n));

    for (auto i = 0u; i < n; ++i)
    {
        // rows
        RuleFactory::createHorizontalLine(puzzle, i, 0, n);
        // columns
        RuleFactory::createVerticalLine(puzzle, 0, i, n);
    }

    for (auto i = 0u; i < n; i += k)
    for (auto j = 0u; j < n; j += k)
    {
        RuleFactory::createBlock(puzzle, i, j, k);
    }
}
