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


/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|							 (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:		YColor.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YColor_h
#define YColor_h

typedef unsigned char uchar;


/**
 * Helper class to define an RGB color.
 **/
class YColor
{
public:
    /**
     * Constructor.
     **/
    YColor( uchar red, uchar green, uchar blue )
	: _red  ( red   )
	, _green( green )
	, _blue ( blue  )
	, _undef( false )
	{}

    /**
     * Default constructor: Create "undefined" color.
     **/
    YColor()
	: _red( 0 ), _green( 0 ), _blue( 0 )
	, _undef( true )
	{}

    /**
     * Return the red component (0: none, 255: bright red).
     **/
    uchar red()   const	{ return _red;   }

    /**
     * Return the green component (0: none, 255: bright green).
     **/
    uchar green() const { return _green; }
    
    /**
     * Return the blue component (0: none, 255: bright blue).
     **/
    uchar blue()  const { return _blue;  }

    /**
     * Return 'true' if this color is undefined.
     **/
    bool isUndefined()   const { return _undef; }

    /**
     * Return 'true' if this color is defined.
     **/
    bool isDefined() const { return ! _undef; }
    
private:
    
    uchar _red;
    uchar _green;
    uchar _blue;

    bool _undef;
};


#endif // YColor_h
