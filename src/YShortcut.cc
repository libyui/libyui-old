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

  File:		YShortcut.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <ctype.h>	// toupper(), tolower()
#include <string.h> 	// strstr()

#define YUILogComponent "ui-shortcuts"
#include "YUILog.h"

#include "YShortcut.h"
#include "YPushButton.h"


// Return the number of elements of an array of any type
#define DIM( ARRAY )	( (int) ( sizeof( ARRAY)/( sizeof( ARRAY[0] ) ) ) )


YShortcut::YShortcut( YWidget * shortcutWidget )
    : _widget( shortcutWidget )
{
    _preferred			= -1;
    _shortcut			= -1;
    _distinctShortcutChars	= -1;
    _conflict			= false;
    _shortcutStringCached	= false;
    _cleanShortcutStringCached	= false;

    YPushButton * button = dynamic_cast<YPushButton *>( shortcutWidget);
    _isButton = ( button != 0 );

    if ( _isButton )
    {
	_isWizardButton = strstr( shortcutWidget->widgetClass(), "WizardButton" );

	// yuiDebug() << shortcutWidget <<  ( _isWizardButton ? " is " : " is not " ) << "a wizard button" << endl;
    }
    else
    {
	_isWizardButton = 0;
    }

    // yuiDebug() << shortcutWidget <<  ( _isButton ? " is " : " is not " ) << "a button" << endl;
}


YShortcut::~YShortcut()
{
}


string
YShortcut::shortcutString()
{
    if ( ! _shortcutStringCached )
    {
	_shortcutString		= getShortcutString();
	_shortcutStringCached	= true;

	// Note: We really need a separate variable here - an empty string
	// might be a valid value!
    }

    return _shortcutString;
}


string
YShortcut::cleanShortcutString()
{
    if ( ! _cleanShortcutStringCached )
    {
	_cleanShortcutString = cleanShortcutString( shortcutString() );
    }

    return _cleanShortcutString;
}


string
YShortcut::cleanShortcutString( string shortcutString )
{
    string::size_type pos = 0;

    while ( ( pos = findShortcutPos( shortcutString, pos ) ) != string::npos )
    {
	shortcutString.erase( pos, ( string::size_type ) 1 );
    }

    return shortcutString;
}


char
YShortcut::preferred()
{
    if ( _preferred < 0 )
    {
	_preferred = normalized( findShortcut( shortcutString() ) );
    }

    return (char) _preferred;
}


char
YShortcut::shortcut()
{
    if ( _shortcut < 0 )
    {
	_shortcut = preferred();
    }

    return (char) _shortcut;
}


void
YShortcut::setShortcut( char newShortcut )
{
    string str = cleanShortcutString();

    if ( newShortcut != YShortcut::None )
    {
	char findme[] = { tolower( newShortcut ), toupper( newShortcut ), 0 };
	string::size_type pos = str.find_first_of( findme );

	if ( pos == string::npos )
	{
	    yuiError() << "Can't find '<< " << newShortcut
		       << "' in " << widgetClass()
		       << " \"" << cleanShortcutString() << "\""
		       << endl;

	    return;
	}

	str.insert( pos,
		    string( 1, shortcutMarker() ) );	// equivalent to 'string( "& " )'
    }

    widget()->setShortcutString( str );

    _shortcutStringCached	= false;
    _cleanShortcutStringCached	= false;
    _shortcut = newShortcut;
}


void
YShortcut::clearShortcut()
{
    setShortcut( YShortcut::None );
}


int
YShortcut::distinctShortcutChars()
{
    if ( _distinctShortcutChars < 0 )	// chache this value - it's expensive!
    {
	// Create and initiazlize "contained" array - what possible shortcut
	// characters are contained in that string?

	bool contained[ sizeof(char) << 8 ];

	for ( int i=0; i < DIM( contained ); i++ )
	    contained[i] = false;


	// Mark characters as contained

	string clean = cleanShortcutString();

	for ( string::size_type pos=0; pos < clean.length(); pos++ )
	{
	    if ( YShortcut::isValid( clean[ pos ] ) )
		contained[ (int) clean[ pos ] ] = true;
	}


	// Count number of contained characters

	_distinctShortcutChars=0;

	for ( int i=0; i < DIM( contained ); i++ )
	{
	    if ( contained[i] )
	    {
		_distinctShortcutChars++;
	    }
	}
    }

    return _distinctShortcutChars;
}


string
YShortcut::getShortcutString()
{
    return getShortcutString( widget() );
}


string
YShortcut::getShortcutString( const YWidget * widget )
{
    if ( ! widget )
	return string( "" );

    return widget->shortcutString();
}


string::size_type
YShortcut::findShortcutPos( const string & str, string::size_type pos )
{
    while ( ( pos = str.find( shortcutMarker(), pos ) ) != string::npos )
    {
	if ( pos+1 < str.length() )
	{
	    if ( str[ pos+1 ] == shortcutMarker() )	// escaped marker? ( "&&" )
	    {
		pos += 2;				// skip this and search for more
	    }
	    else
		return pos;
	}
	else
	{
	    // A pathological case: The string ends with '& '.
	    // This is invalid anyway, but prevent endless loop even in this case.
	    return string::npos;
	}
    }

    return string::npos;	// not found
}


char
YShortcut::findShortcut( const string & str, string::size_type pos )
{
    pos = findShortcutPos( str, pos );

    return pos == string::npos ? (char) 0 : str[ pos+1 ];
}


bool
YShortcut::isValid( char c )
{
    if ( c >= 'a' && c <= 'z' )	return true;
    if ( c >= 'A' && c <= 'Z' )	return true;
    if ( c >= '0' && c <= '9' )	return true;
    return false;
}


char
YShortcut::normalized( char c )
{
    if ( c >= 'a' && c <= 'z' )	return c - 'a' + 'A';
    if ( c >= 'A' && c <= 'Z' )	return c;
    if ( c >= '0' && c <= '9' )	return c;
    return (char) 0;
}

