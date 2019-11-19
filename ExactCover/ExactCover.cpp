#include "pch.h"
#include "ExactCover.h"

using namespace ExactCover;

std::vector<Element *> Solver::search(Head * head, unsigned int k, std::vector<Element *> solution, const Sudoku::ILogger & logger)
{
    if (head->right == head)
    {
        return solution;
    }

    auto column = selectColumn(head);
    auto r = column->down;
    auto c = static_cast<Element *>(column);

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

Column * Solver::selectColumn(Head * head)
{
    auto i = static_cast<Column *>(head->right);
    auto minElement = i;

    while (i != head)
    {
        if (i->size < minElement->size)
        {
            minElement = i;
        }
        i = static_cast<Column *>(i->right);
    }

    return minElement;
}

void Solver::cover(Column * c)
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

void Solver::uncover(Column * c)
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

void Solver::log(std::vector<Element *> solution, const Sudoku::ILogger & logger)
{
    for (const auto * element : solution)
    {
        std::string str = element->column->name;
        auto i = element->right;
        while (i != element)
        {
            str.append(i->column->name);
            i = i->right;
        }
        logger(str);
    }
}