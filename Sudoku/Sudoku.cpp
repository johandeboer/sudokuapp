#include "pch.h"
#include "Sudoku.h"
#include "Cell.h"
#include "Rule.h"
#include "Overlap.h"

using namespace Sudoku;

Puzzle::Puzzle(std::size_t columns, std::size_t rows, unsigned int digits, ILogger * logger) :
    mColumns(columns),
    mRows(rows),
    mDigits(digits),
    mLogger(logger)
{
    for (std::size_t i = 0; i < rows * columns; i++)
    {
        mGrid.emplace_back(std::make_shared<Cell>(digits));
    }
}

std::pair<size_t, size_t> Puzzle::toRowColumn(std::shared_ptr<Cell> cell)
{
    auto iter = std::find(mGrid.begin(), mGrid.end(), cell);
    auto index = std::distance(mGrid.begin(), iter);
    auto column = index % mColumns;
    auto row = index / mColumns;
    return { row, column };
}

std::string Puzzle::toRowColString(std::shared_ptr<Cell> cell)
{
    auto [row, col] = toRowColumn(cell);
    return "[" + std::to_string(row) + "," + std::to_string(col) + "]";
}

std::string Sudoku::Puzzle::toString(const std::vector<std::shared_ptr<Cell>> & cells)
{
    std::string str;
    for (const auto & cell : cells)
    {
        str.append(toRowColString(cell) + " ");
    }
    str.pop_back();
    return str;
}

const std::vector<std::shared_ptr<Rule>> & Puzzle::rules()
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

// FIXME: this rule is a specialization of sweepUniques
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

            mLogger->log("unique: found " + std::to_string(digit) + " @ " + toRowColString(cell));
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
        // FIXME
        changes += sweepOverlap(overlap.overlap(), overlap.nonOverlapA(), overlap.nonOverlapB());
        if (changes > 0)
            return changes;
        changes += sweepOverlap(overlap.overlap(), overlap.nonOverlapB(), overlap.nonOverlapA());
        if (changes > 0)
            return changes;
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
        if (count < 2) // FIXME: why 2?
        {
            continue;
        }

        auto inA = std::any_of(nonOverlapA.begin(), nonOverlapA.end(), isSet);
        if (inA)
        {
            continue;
        }

        std::string logStr;
        for (const auto & cell : nonOverlapB)
        {
            if (cell->isSet(digit))
            {
                cell->unset(digit);
                ++changes;

                logStr += toRowColString(cell) + " ";
            }
        }
        if (!logStr.empty())
        {
            mLogger->log("overlap: remove " + std::to_string(digit) + " @ " + logStr);
            return changes; // FIXME
        }
    }

    return changes;
}