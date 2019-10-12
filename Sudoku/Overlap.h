#pragma once

#include "Rule.h"

namespace Sudoku
{

class Overlap
{
public:
    Overlap(const std::shared_ptr<Rule> & ruleA, const std::shared_ptr<Rule> & ruleB);

    Overlap(const Overlap &) = delete;
    Overlap(Overlap &&) = default;
    Overlap & operator=(const Overlap &) = delete;
    Overlap & operator=(Overlap &&) = default;

    static bool overlaps(const std::shared_ptr<Rule> & ruleA, const std::shared_ptr<Rule> & ruleB);

    const std::vector<std::shared_ptr<Cell>> & overlap() const { return mOverlap; }
    const std::vector<std::shared_ptr<Cell>> & nonOverlapA() const { return mNonOverlapA; }
    const std::vector<std::shared_ptr<Cell>> & nonOverlapB() const { return mNonOverlapB; }

private:
    std::shared_ptr<Rule> mRuleA;
    std::shared_ptr<Rule> mRuleB;

    std::vector<std::shared_ptr<Cell>> mOverlap;
    std::vector<std::shared_ptr<Cell>> mNonOverlapA;
    std::vector<std::shared_ptr<Cell>> mNonOverlapB;
};

}