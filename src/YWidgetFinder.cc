/*
  Copyright (C) 2017 SUSE LLC

  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) version 3.0 of the License. This library
  is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
  License for more details. You should have received a copy of the GNU
  Lesser General Public License along with this library; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
  Floor, Boston, MA 02110-1301 USA
*/

// boost::erase_all
#include <boost/algorithm/string.hpp>

#include "YDialog.h"
#include "YWidget.h"
#include "YWidgetID.h"

#include "YWidgetFinder.h"

// internal helper methods
static void find_widgets_by_label_rec(YWidget *w, const std::string &label, WidgetArray &array);
static void find_widgets_by_id_rec(YWidget *w, const std::string &id, WidgetArray &array);
static void find_widgets_by_type_rec(YWidget *w, const std::string &type, WidgetArray &array);
static void find_all_widgets_rec(YWidget *w, WidgetArray &array);

WidgetArray YWidgetFinder::by_label(const std::string &label)
{
    WidgetArray ret;
    find_widgets_by_label_rec(YDialog::topmostDialog(), label, ret);
    return ret;
}

WidgetArray YWidgetFinder::by_id(const std::string &id)
{
    WidgetArray ret;
    find_widgets_by_id_rec(YDialog::topmostDialog(), id, ret);
    return ret;
}

WidgetArray YWidgetFinder::by_type(const std::string &type)
{
    WidgetArray ret;
    find_widgets_by_type_rec(YDialog::topmostDialog(), type, ret);
    return ret;
}

WidgetArray YWidgetFinder::all()
{
    WidgetArray ret;
    find_all_widgets_rec(YDialog::topmostDialog(), ret);
    return ret;
}


void find_all_widgets_rec(YWidget *w, WidgetArray &array)
{
    if (!w) return;
    array.push_back(w);

    // search also in the children widgets
    // YWidget provides begin() and end() so we can iterate it just like any container
    for(YWidget *child: *w)
    {
        find_all_widgets_rec(child, array);
    };
}

void find_widgets_by_label_rec(YWidget *w, const std::string &label, WidgetArray &array)
{
    if (!w) return;
    // check the widget label if it is defined
    if (w->propertySet().contains("Label"))
    {
        std::string widget_label = w->getProperty("Label").stringVal();
        boost::erase_all(widget_label, "&");

        if (widget_label == label)
        {
            array.push_back(w);
        }
    }

    // search also in the children widgets
    // YWidget provides begin() and end() so we can iterate it just like any container
    for(YWidget *child: *w)
    {
        find_widgets_by_label_rec(child, label, array);
    };
}

void find_widgets_by_id_rec(YWidget *w, const std::string &id, WidgetArray &array)
{
    if (!w) return;
    // check the widget ID if it is defined
    if (w->hasId() && w->id()->toString() == id) {
        array.push_back(w);
    }

    // search also in the children widgets
    // YWidget provides begin() and end() so we can iterate it just like any container
    for(YWidget *child: *w)
    {
        find_widgets_by_id_rec(child, id, array);
    };
}

void find_widgets_by_type_rec(YWidget *w, const std::string &type, WidgetArray &array)
{
    if (!w) return;
    auto propSet = w->propertySet();

    if (propSet.contains("WidgetClass") && w->getProperty("WidgetClass").stringVal() == type)
        array.push_back(w);

    // search also in the children widgets
    // YWidget provides begin() and end() so we can iterate it just like any container
    for(YWidget *child: *w)
    {
        find_widgets_by_type_rec(child, type, array);
    };
}

