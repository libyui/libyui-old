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

#ifndef YWidgetFinder_h
#define YWidgetFinder_h

#include <string>
#include <vector>

class YWidget;

typedef std::vector<YWidget*> WidgetArray;

class YWidgetFinder
{

public:

    static WidgetArray by_label(const std::string &label);

    static WidgetArray by_id(const std::string &id);

    static WidgetArray by_type(const std::string &type);

    static WidgetArray all();

};

#endif
