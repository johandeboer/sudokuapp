#include "pch.h"
#include "Overlap.h"

using namespace Sudoku;

Overlap::Overlap(const std::shared_ptr<Rule> & ruleA, const std::shared_ptr<Rule> & ruleB) :
    mRuleA(ruleA),
    mRuleB(ruleB)
{
}

bool Overlap::overlaps(const std::shared_ptr<Rule> & ruleA, const std::shared_ptr<Rule> & ruleB)
{
    auto all = ruleA->cells();
    all.insert(all.begin(), ruleB->cells().begin(), ruleB->cells().end());
    std::sort(all.begin(), all.end());
    auto last = std::unique(all.begin(), all.end());
    return last != all.end();
}
