// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Widget.hpp"

void tnt::Widget::setParent(Widget *w)
{
    parent = w;
}

tnt::Widget *tnt::Widget::getParent() const noexcept
{
    return parent;
}

bool tnt::Widget::Active() const noexcept
{
    return active;
}
