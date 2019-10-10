#include "pch.h"
#include "RuleFactory.h"

using namespace Sudoku;

void RuleFactory::createHorizontalLine(Sudoku::Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int count)
{
    auto line = HorizontalRule();
    for (auto i = 0u; i < count; ++i)
    {
        line.addCell(puzzle.cell(row, column + i));
    }
    puzzle.addRule(std::move(line));
}

void RuleFactory::createVerticalLine(Sudoku::Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int count)
{
    auto line = VerticalRule();
    for (auto i = 0u; i < count; ++i)
    {
        line.addCell(puzzle.cell(row + i, column));
    }
    puzzle.addRule(std::move(line));
}

void RuleFactory::createBlock(Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int size)
{
    auto block = BlockRule();
    for (auto i = 0u; i < size; ++i)
    for (auto j = 0u; j < size; ++j)
    {
        block.addCell(puzzle.cell(row + i, column + j));
    }
    puzzle.addRule(std::move(block));
}
