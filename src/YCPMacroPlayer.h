/****************************************************************************

Copyright (c) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, contact Novell, Inc.

To contact Novell about this file by physical or electronic mail,
you may find current contact information at www.novell.com

****************************************************************************

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
