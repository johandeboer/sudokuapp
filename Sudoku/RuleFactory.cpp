#include "pch.h"
#include "RuleFactory.h"

using namespace Sudoku;

Rule RuleFactory::createLine(Puzzle & puzzle, unsigned int startIndex, unsigned int count, unsigned int stride)
{
    auto line = Rule();
    for (auto i = 0u; i < count; ++i)
    {
        line.addCell(puzzle.cell(startIndex + i * stride));
    }
    return line;
}

HorizontalRule Sudoku::RuleFactory::createHorizontalLine(Sudoku::Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int count)
{
    auto line = HorizontalRule();
    for (auto i = 0u; i < count; ++i)
    {
        line.addCell(puzzle.cell(row, column + i));
    }
    return line;
}

VerticalRule Sudoku::RuleFactory::createVerticalLine(Sudoku::Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int count)
{
    auto line = VerticalRule();
    for (auto i = 0u; i < count; ++i)
    {
        line.addCell(puzzle.cell(row + i, column));
    }
    return line;
}

BlockRule RuleFactory::createBlock(Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int size)
{
    auto block = BlockRule();
    for (auto i = 0u; i < size; ++i)
        for (auto j = 0u; j < size; ++j)
        {
            block.addCell(puzzle.cell(row + i, column + j));
        }
    return block;
}
