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

  File:		YCPMacroPlayer.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPMacroPlayer_h
#define YCPMacroPlayer_h

#include <string>
#include <ycp/YBlock.h>
#include <yui/YMacroPlayer.h>


class YCPMacroPlayer: public YMacroPlayer
{
public:

    /**
     * Constructor
     **/
    YCPMacroPlayer();

    /**
     * Destructor
     **/
    virtual ~YCPMacroPlayer();

    /**
     * Play a macro from the specified macro file.
     *
     * Implemented from YMacroPlayer.
     **/
    virtual void play( const string & macroFile );

    /**
     * Return 'true' if a macro is currently being played.
     *
     * Implemented from YMacroPlayer.
     **/ 
    virtual bool playing() const;
    
    /**
     * Play the next block from the current macro, if there is one playing.
     *
     * Implemented from YMacroPlayer.
     **/
    virtual void playNextBlock();

    /**
     * Report error status
     **/
    bool error() const { return _error; }

    /**
     * Returns "true" if the execution of the entire macro is finished.
     * Undefined on error, so check for error() first!
     **/
    bool finished();

    /**
     * Rewind macro execution - prepare to redo from start
     **/
    void rewind();

protected:

    //
    // Data members
    //
    
    YBlockPtr	_macro;
    bool	_playing;
    bool	_error;
    int		_nextBlockNo;
};

#endif // YCPMacroPlayer_h
