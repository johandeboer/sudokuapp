#include "pch.h"
#include "Overlap.h"

using namespace Sudoku;

Overlap::Overlap(const std::shared_ptr<Rule> & ruleA, const std::shared_ptr<Rule> & ruleB) :
    mRuleA(ruleA),
    mRuleB(ruleB)
{
    auto contains = [](const std::vector<std::shared_ptr<Cell>> & cells, const std::shared_ptr<Cell> & cell) {
        auto iter = std::find(cells.begin(), cells.end(), cell);
        return iter != cells.end();
    };

    for (const auto & cellA : ruleA->cells())
    {
        if (contains(ruleB->cells(), cellA))
        {
            mOverlap.push_back(cellA);
        }
        else
        {
            mNonOverlapA.push_back(cellA);
        }
    }

    for (const auto & cellB : ruleB->cells())
    {
        if (!contains(mOverlap, cellB))
        {
            mNonOverlapB.push_back(cellB);
        }
    }
}

bool Overlap::overlaps(const std::shared_ptr<Rule> & ruleA, const std::shared_ptr<Rule> & ruleB)
{
    auto all = ruleA->cells();
    all.insert(all.begin(), ruleB->cells().begin(), ruleB->cells().end());
    std::sort(all.begin(), all.end());
    auto last = std::unique(all.begin(), all.end());
    auto coverCount = std::distance(last, all.end());
    return coverCount > 1;
}
