#include "pch.h"
#include "Element.h"

using namespace ExactCover;

Element::Element() :
    left(this),
    right(this),
    up(this),
    down(this)
{
}

Element::Element(Column * theColumn, ExactCover::Row * theRow) :
    Element()
{
    column = theColumn;
    row = theRow;
    down = column->down;
    column->down->up = this;
    up = column;
    column->down = this;
    column->size++;
}