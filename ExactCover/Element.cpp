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

Element::Element(std::shared_ptr<Column> theColumn, std::shared_ptr<Row> theRow) :
    Element()
{
    column = theColumn;
    row = theRow;
    down = column->down;
    column->down->up = std::shared_ptr<Element>(this);
    up = column;
    column->down = std::shared_ptr<Element>(this);
    column->size++;
}