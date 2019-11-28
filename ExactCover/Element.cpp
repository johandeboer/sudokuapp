#include "pch.h"
#include "Element.h"

using namespace ExactCover;

Element::Element(size_t row, size_t col) :
    rowIndex(row),
    colIndex(col)
{
}

void Column::addElement(Element * element)
{
    element->column = this;
    element->down = down;
    down->up = element;
    element->up = this;
    down = element;
    size++;
}
