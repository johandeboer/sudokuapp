#include "pch.h"
#include "RuleFactory.h"

using namespace Sudoku;

void RuleFactory::createHorizontalLine(const std::shared_ptr<Puzzle> & puzzle, unsigned int row, unsigned int column, unsigned int count)
{
    auto line = std::make_shared<HorizontalRule>();
    for (auto i = 0u; i < count; ++i)
    {
        auto cell = puzzle->cell(row, column + i);
        line->addCell(cell);
        cell->addRule(line);
    }
    puzzle->addRule(std::move(line));
}

void RuleFactory::createVerticalLine(const std::shared_ptr<Puzzle> & puzzle, unsigned int row, unsigned int column, unsigned int count)
{
    auto line = std::make_shared<VerticalRule>();
    for (auto i = 0u; i < count; ++i)
    {
        auto cell = puzzle->cell(row + i, column);
        line->addCell(cell);
        cell->addRule(line);
    }
    puzzle->addRule(std::move(line));
}

void RuleFactory::createBlock(const std::shared_ptr<Puzzle> & puzzle, unsigned int row, unsigned int column, unsigned int size)
{
    auto block = std::make_shared<BlockRule>();
    for (auto i = 0u; i < size; ++i)
    for (auto j = 0u; j < size; ++j)
    {
        auto cell = puzzle->cell(row + i, column + j);
        block->addCell(cell);
        cell->addRule(block);
    }
    puzzle->addRule(std::move(block));
}
