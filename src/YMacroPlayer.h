/*
  |****************************************************************************
  |
  | Copyright (c) 2000 - 2012 Novell, Inc.
  | All Rights Reserved.
  |
  | This program is free software; you can redistribute it and/or
  | modify it under the terms of version 2 of the GNU General Public License as
  | published by the Free Software Foundation.
  |
  | This program is distributed in the hope that it will be useful,
  | but WITHOUT ANY WARRANTY; without even the implied warranty of
  | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
  | GNU General Public License for more details.
  |
  | You should have received a copy of the GNU General Public License
  | along with this program; if not, contact Novell, Inc.
  |
  | To contact Novell about this file by physical or electronic mail,
  | you may find current contact information at www.novell.com
  |
  |****************************************************************************
*/


/*-/

  File:		YMacroPlayer.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YMacroPlayer_h
#define YMacroPlayer_h

#include <string>

/**
 * Abstract base class for macro player.
 *
 * Applications should not use this directly, but the static methods in YMacro.
 **/
class YMacroPlayer
{
    friend class YMacro;

protected:
    /**
     * Constructor
     **/
    YMacroPlayer() {}

public:
    /**
     * Destructor
     **/
    virtual ~YMacroPlayer() {}

    /**
     * Play a macro from the specified macro file.
     **/
    virtual void play( const std::string & macroFile ) = 0;

    /**
     * Play the next block from the current macro, if there is one playing.
     **/
    virtual void playNextBlock() = 0;

    /**
     * Return 'true' if a macro is currently being played.
     **/
    virtual bool playing() const = 0;
};

#endif // YMacroPlayer_h
