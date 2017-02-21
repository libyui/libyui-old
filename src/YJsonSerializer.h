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

#ifndef YJsonSerializer_h
#define YJsonSerializer_h

#include <iostream>
#include <vector>

class YWidget;

namespace Json {
    class Value;
}

class YJsonSerializer
{

public:

    // serialize one widget (by default recursively with all children)
    static void serialize(YWidget *, std::ostream &output, bool recursive = true);

    // serialize widget array (by default recursively with all children)
    static void serialize(const std::vector<YWidget*> &widgets, std::ostream &output, bool recursive = true);

    // save the JSON value as a text into the output stream
    static void save(const Json::Value &json, std::ostream &output);
};

#endif
