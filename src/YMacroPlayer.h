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
    virtual void play( const string & macroFile ) = 0;

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
