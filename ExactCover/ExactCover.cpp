#include "pch.h"
#include "ExactCover.h"

using namespace ExactCover;

std::vector<Solution> Solver::search(std::shared_ptr<Head> head, const Sudoku::ILogger & logger)
{
    mSolutions = {};
    search(head, 0, {}, logger);
    return mSolutions;
}

Solution Solver::search(std::shared_ptr<Head> head, unsigned int k, Solution solution, const Sudoku::ILogger & logger)
{
    if (head->right == head)
    {
        mSolutions.push_back(solution);
        return solution;
    }

    auto column = selectColumn(head);
    auto r = column->down;
    auto c = std::static_pointer_cast<Element>(column);

    cover(column);

    while (r != c)
    {
        solution.push_back(r);

        auto j = r->right;
        while (j != r)
        {
            cover(j->column);
            j = j->right;
        }

        solution = search(head, k + 1, solution, logger);

        r = solution.back();
        solution.pop_back();
        column = r->column;

        j = r->left;
        while (j != r)
        {
            uncover(j->column);
            j = j->left;
        }

        r = r->down;
    }

    uncover(column);

    return solution;
}

std::shared_ptr<Column> Solver::selectColumn(std::shared_ptr<Head> head)
{
    auto i = std::static_pointer_cast<Column>(head->right);
    auto minElement = i;

    while (i != head)
    {
        if (i->size < minElement->size)
        {
            minElement = i;
        }
        i = std::static_pointer_cast<Column>(i->right);
    }

    return minElement;
}

void Solver::cover(std::shared_ptr<Column> c)
{
    c->right->left = c->left;
    c->left->right = c->right;

    auto i = c->down;
    while (i != c)
    {
        auto j = i->right;
        while (j != i)
        {
            j->down->up = j->up;
            j->up->down = j->down;
            j->column->size--;
            j = j->right;
        }
        i = i->down;
    }
}

void Solver::uncover(std::shared_ptr<Column> c)
{
    auto i = c->up;
    while (i != c)
    {
        auto j = i->left;
        while (j != i)
        {
            j->column->size++;
            j->down->up = j;
            j->up->down = j;
            j = j->left;
        }
        i = i->up;
    }
    c->right->left = c;
    c->left->right = c;
}

void Solver::log(Solution solution, const Sudoku::ILogger & logger)
{
    for (const auto element : solution)
    {
        auto str = element->row->Name;
        str.append(" :");
        str.append(element->column->Name);
        auto i = element->right;
        while (i != element)
        {
            str.append(i->column->Name);
            i = i->right;
        }
        logger(str);
    }
}