/**************************************************************************
Copyright (C) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************/


#ifndef YIconLoader_H
#define YIconLoader_H

#include <string>
#include <list>


using namespace std;

class YIconLoader 
{
public:

    YIconLoader();
    ~YIconLoader();

    string findIcon( string name );

    //FIXME: these two are here for compatibility reasons
    // deprecate them in due course and treat base path just 
    // like any other search path
    void setIconBasePath( string path );
    string iconBasePath() const;

    void addIconSearchPath( string path );

private:

    string _iconBasePath;
    list <string> icon_dirs;

    bool fileExists( string fname );
};

#endif
