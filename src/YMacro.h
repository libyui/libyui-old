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

  File:		YMacro.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YMacro_h
#define YMacro_h

#include <string>

class YMacroRecorder;
class YMacroPlayer;
using std::string;


/**
 * Simple access to macro recording and playing.
 *
 * This class stores an instance of a macro recorder and a macro player.
 * Since both YMacroRecorder and YMacroPlayer are abstract base classes,
 * derived classes from either of them have to be instantiated and set
 * (setRecorder(), setPlayer()) from the outside for anything to happen. Until
 * that point, none of the macro operations here do anything (but also don't
 * throw any error or exception).
 **/
class YMacro
{
private:
    YMacro() {}
    ~YMacro() {}

public:

    /**
     * Set a macro recorder.
     *
     * This needs to be done from the outside since YMacroRecorder is an
     * abstract base class, i.e., it needs to be derived to be instantiated.
     **/
    static void setRecorder( YMacroRecorder * recorder );

    /**
     * Set a macro player.
     *
     * This needs to be done from the outside since YMacroRecorder is an
     * abstract base class, i.e., it needs to be derived to be instantiated.
     **/
    static void setPlayer( YMacroPlayer * player );

    /**
     * Record a macro to the specified macro file.
     **/
    static void record( const string & macroFile );

    /**
     * End macro recording.
     **/
    static void endRecording();

    /**
     * Return 'true' if a macro is currently being recorded.
     **/
    static bool recording();

    /**
     * Play a macro from the specified macro file.
     **/
    static void play( const string & macroFile );

    /**
     * Play the next block from the current macro, if there is one playing.
     **/
    static void playNextBlock();

    /**
     * Return 'true' if a macro is currently being played.
     **/ 
    static bool playing();

    /**
     * Return the current macro recorder or 0 if there is none.
     **/
    static YMacroRecorder * recorder() { return _recorder; }
    
    /**
     * Return the current macro player or 0 if there is none.
     **/
    static YMacroPlayer * player() { return _player; }

    /**
     * Delete the current macro recorder if there is one.
     **/
    static void deleteRecorder();

    /**
     * Delete the current macro player if there is one.
     **/
    static void deletePlayer();

private:

    static YMacroRecorder * _recorder;
    static YMacroPlayer   * _player;
};



#endif // YMacro_h
