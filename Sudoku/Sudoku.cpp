#include "pch.h"
#include "Sudoku.h"
#include "Cell.h"
#include "Rule.h"
#include "Overlap.h"

using namespace Sudoku;

Puzzle::Puzzle(std::size_t columns, std::size_t rows, unsigned int digits) :
    mColumns(columns),
    mRows(rows),
    mDigits(digits)
{
    for (std::size_t i = 0; i < rows * columns; i++)
    {
        mGrid.emplace_back(std::make_shared<Cell>(digits));
    }
}

std::pair<size_t, size_t> Sudoku::Puzzle::toRowColumn(std::shared_ptr<Cell> cell)
{
    auto iter = std::find(mGrid.begin(), mGrid.end(), cell);
    auto index = std::distance(mGrid.begin(), iter);
    auto column = index % mColumns;
    auto row = index / mColumns;
    return { row, column };
}

const std::vector<std::shared_ptr<Rule>> & Sudoku::Puzzle::rules()
{
    return mRules;
}

void Puzzle::addRule(std::shared_ptr<Rule> && newRule)
{
    for (const auto & rule : mRules)
    {
        if (Overlap::overlaps(rule, newRule))
        {
            mOverlaps.emplace_back(rule, newRule);
        }
    }

    mRules.emplace_back(std::move(newRule));
}

std::shared_ptr<Cell> Puzzle::cell(std::size_t index)
{
    return mGrid[index];
}

std::shared_ptr<Cell> Puzzle::cell(std::size_t row, std::size_t column)
{
    return mGrid[row * mColumns + column];
}

void Puzzle::removeCell(std::size_t row, std::size_t column)
{
    assert(mGrid[row * mColumns + column]->rules().size() == 0);
    mGrid[row * mColumns + column].reset();
}

unsigned int Puzzle::sweepClues()
{
    unsigned int changes = 0u;
    for (const auto & cell : mGrid)
    {
        if (cell != nullptr && !cell->hasClue())
        {
            auto clue = cell->isSolved();
            if (clue.has_value())
            {
                cell->setClue(clue.value());
                changes++;
            }
        }
    }

    return changes;
}

unsigned int Puzzle::sweepUniques()
{
    auto changes = 0u;

    for (const auto & rule : mRules)
    {
        auto [cell, digit] = findUnique(rule);
        if (cell != nullptr)
        {
            cell->setClue(digit);
            changes++;
        }
    }

    return changes;
}

std::pair<std::shared_ptr<Cell>, unsigned int> Puzzle::findUnique(const std::shared_ptr<Rule> & rule)
{
    for (auto digit = 1u; digit <= mDigits; digit++)
    {
        std::shared_ptr<Cell> foundCell = nullptr;
        bool unique = true;

        for (const auto & cell : rule->cells())
        {
            if (cell->clue() == digit)
            {
                break;
            }

            if (cell->hasClue())
            {
                continue;
            }

            if (cell->isSet(digit))
            {
                if (foundCell != nullptr)
                {
                    unique = false;
                    break;
                }

                foundCell = cell;
            }
        }

        if (unique && foundCell != nullptr)
        {
            return { foundCell, digit };
        }
    }

    return { nullptr, 0 };
}

unsigned int Puzzle::sweepOverlaps()
{
    auto changes = 0u;

    for (const auto & overlap : mOverlaps)
    {
        changes += sweepOverlap(overlap.overlap(), overlap.nonOverlapA(), overlap.nonOverlapB());
        changes += sweepOverlap(overlap.overlap(), overlap.nonOverlapB(), overlap.nonOverlapA());
    }

    return changes;
}

unsigned int Puzzle::sweepOverlap(
    const std::vector<std::shared_ptr<Cell>> & overlap,
    const std::vector<std::shared_ptr<Cell>> & nonOverlapA,
    const std::vector<std::shared_ptr<Cell>> & nonOverlapB)
{
    auto changes = 0;

    for (auto digit = 1u; digit <= mDigits; digit++)
    {
        auto isSet = [digit](const auto & cell) { return cell->isSet(digit); };

        auto count = std::count_if(overlap.begin(), overlap.end(), isSet);
        if (count < 2)
        {
            continue;
        }

        auto inA = std::any_of(nonOverlapA.begin(), nonOverlapA.end(), isSet);
        if (inA)
        {
            continue;
        }

        for (const auto & cell : nonOverlapB)
        {
            if (cell->isSet(digit))
            {
                cell->unset(digit);
                ++changes;
            }
        }
    }

    return changes;
}