#include "pch.h"
#include "SudokuFactory.h"
#include "Rule.h"
#include "Cell.h"
#include "RuleFactory.h"
#include <sstream>

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
        RuleFactory::createHorizontalLine(puzzle, i, 0, static_cast<unsigned int>(n));
        RuleFactory::createVerticalLine(puzzle, 0, i, static_cast<unsigned int>(n));
    }

    for (auto i = 0u; i < n; i += k)
    for (auto j = 0u; j < n; j += k)
    {
        RuleFactory::createBlock(puzzle, i, j, k);
    }
}

void SudokuFactory::makeMixRules(const std::shared_ptr<Puzzle> & puzzle, std::stringstream * ss)
{
    auto n = 9;
    auto k = 3;

    std::string line;
    auto m = 8u;
    auto blockRules = std::vector<std::shared_ptr<BlockRule>>();
    for (auto i = 0u; i < m; i++)
    {
        blockRules.emplace_back(std::make_shared<BlockRule>());
        puzzle->addRule(blockRules.back());
    }

    for (auto row = 0u; row < n; row++)
    {
        *ss >> line;

        for (auto column = 0u; column < n; ++column)
        {
            auto ch = line[column];
            if (ch == '-')
            {
                continue;
            }

            auto blockRule = blockRules[ch - '1'];
            auto cell = puzzle->cell(row, column);
            blockRule->addCell(cell);
            cell->addRule(blockRule);
        }
    }

    for (auto i = 0u; i < n; ++i)
    {
        RuleFactory::createHorizontalLine(puzzle, i, 0, static_cast<unsigned int>(n));
        RuleFactory::createHorizontalLine(puzzle, i + 6, 6, static_cast<unsigned int>(n));
        RuleFactory::createVerticalLine(puzzle, 0, i, static_cast<unsigned int>(n));
        RuleFactory::createVerticalLine(puzzle, 6, i + 6, static_cast<unsigned int>(n));
    }

    for (auto i = 0u; i < n; i += k)
    for (auto j = 0u; j < n; j += k)
    {
        RuleFactory::createBlock(puzzle, i + 6, j + 6, k);
    }

    RuleFactory::createDiagonalLine(puzzle, 6 * puzzle->columns() + 6, n, puzzle->columns() + 1);
    RuleFactory::createDiagonalLine(puzzle, 6 * puzzle->columns() + 14, n, puzzle->columns() - 1);
}
