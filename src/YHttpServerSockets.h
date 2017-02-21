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

#ifndef YHttpServerSockets_h
#define YHttpServerSockets_h

#include <vector>

class YHttpServerSockets
{

public:
    
    typedef std::vector<int> Set;

    Set read() {return _read;}
    Set write() {return _write;}
    Set exception() {return _exception;}

    void add_read(int s) {_read.push_back(s);}
    void add_write(int s)  {_write.push_back(s);}
    void add_exception(int s)  {_exception.push_back(s);}

    bool empty() { return _read.empty() && _write.empty() && _exception.empty();}

private:
    
    Set _read;
    Set _write;
    Set _exception;
};

#endif
