/*
  Copyright (C) 2000-2012 Novell, Inc
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


/*-/

  File:		YShortcut.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef YShortcut_h
#define YShortcut_h

#include "YWidget.h"
#include <string>
#include <vector>

class YItem;


/**
 * Helper class for shortcut management:
 * This class holds data about the shortcut for one single widget.
 **/
class YShortcut
{
public:
    /**
     * Constructor
     **/
    YShortcut( YWidget *shortcut_widget );

    /**
     * Destructor
     **/
    virtual ~YShortcut();

    /**
     * Marker for "no shortcut"
     **/
    enum { None = 0 };

    /**
     * Returns the YWidget this shortcut data belong to.
     **/
    YWidget * widget() const { return _widget; }

    /**
     * Returns the textual representation of the widget class of the widget
     * this shortcut data belongs to.
     **/
    const char * widgetClass() const { return widget()->widgetClass(); }

    /**
     * Returns 'true' if the widget that is associated with this shortcut is a
     * button (derived from YPushButton).
     **/
    bool isButton() const { return _isButton; }

    /**
     * Returns 'true' if the widget that is associated with this shortcut is a
     * wizard button (one of the navigation buttons of a wizard).
     **/
    bool isWizardButton() const { return _isWizardButton; }

    /**
     * Returns the complete shortcut string (which may or may not contain "&"),
     * i.e. the value of the widget's shortcut property. For PushButtons, this
     * is the label on the button ( e.g., "&Details..." ), for other widgets
     * usually the caption above it.
     *
     * This value is chached, i.e. this isn't a too expensive operation.
     **/
    std::string shortcutString();

    /**
     * Returns the shortcut string ( from the widget's shortcut property )
     * without any "&" markers.
     **/
    std::string cleanShortcutString();

    /**
     * Static version of the above for general use:
     * Returns the specified string without any "&" markers.
     **/
    static std::string cleanShortcutString( std::string shortcutString );

    /**
     * The preferred shortcut character, i.e. the character that had been
     * preceded by "&" before checking / resolving conflicts began.
     **/
    char preferred();

    /**
     * The actual shortcut character.
     *
     * This may be different from preferred() if it is overridden.
     **/
    char shortcut();

    /**
     * Set (override) the shortcut character.
     **/
    virtual void setShortcut( char newShortcut );

    /**
     * Clear the shortcut: Override the shortcut character with nothing.
     * This may happen if a conflict cannot be resolved.
     **/
    void clearShortcut();

    /**
     * Query the internal 'conflict' marker. This class doesn't care about that
     * flag, it just stores it for the convenience of higher-level classes.
     **/
    bool conflict() { return _conflict; }

    /**
     * Set or unset the internal 'conflict' marker.
     **/
    void setConflict( bool newConflictState = true ) { _conflict = newConflictState; }

    /**
     * Obtain the number of distinct valid shortcut characters in the shortcut
     * string, i.e. how many different shortcuts that widget could get.
     **/
    int distinctShortcutChars();

    /**
     * Return true if this shortcut contains any character that would be valid
     * as a shortcut character.
     **/
    bool hasValidShortcutChar();

    /**
     * Static function: Returns the character used for marking keyboard
     * shortcuts.
     **/
    static char shortcutMarker() { return '&'; }

    /**
     * Static function: Find the next occurrence of the shortcut marker ('&')
     * in a string, beginning at starting position start_pos.
     *
     * Returns string::npos if not found or the position of the shortcut marker
     * (not the shortcut character!) if found.
     **/
    static std::string::size_type findShortcutPos( const std::string & str, std::string::size_type start_pos = 0 );

    /**
     * Static function: Find the next shortcut marker in a string, beginning at
     * starting position start_pos.
     *
     * Returns the shortcut character or 0 if none found.
     **/
    static char findShortcut( const std::string & str, std::string::size_type start_pos = 0 );

    /**
     * Returns 'true' if 'c' is a valid shortcut character, i.e. [a-zA-Z0-9],
     * 'false' otherwise.
     **/
    static bool isValid( char c );

    /**
     * Return the normalized version of shortcut character 'c', i.e. a
     * lowercase letter or a digit [a-z0-9]. Returns 0 if 'c' is invalid.
     **/
    static char normalized( char c );

    /**
     * Obtain a widget's shortcut property - the string that contains "&" to
     * designate a shortcut.
     **/
    static std::string getShortcutString( const YWidget * widget );


protected:

    /**
     * Obtain the the shortcut property of this shortcut's widget - the string
     * that contains "&" to designate a shortcut.
     **/
    virtual std::string getShortcutString();


    // Data members

    YWidget *	_widget;
    std::string	_shortcutString;
    bool	_shortcutStringCached;

    std::string	_cleanShortcutString;
    bool	_cleanShortcutStringCached;

    int		_preferred;	// int to enable initializing with invalid char (-1)
    int		_shortcut;	// int to enable initializing with invalid char (-1)

    bool	_conflict;
    bool	_isButton;
    bool	_isWizardButton;
    int		_distinctShortcutChars;
};



/**
 * Special case for widgets that can have multiple shortcuts based on items
 * (like YDumbTab)
 **/
class YItemShortcut: public YShortcut
{
public:
    /**
     * Constructor.
     **/
    YItemShortcut( YWidget * widget, YItem * item )
	: YShortcut( widget )
	, _item( item )
	{}

    /**
     * Destructor.
     **/
    virtual ~YItemShortcut() {}

    /**
     * Return the associated item.
     **/
    YItem * item() const { return _item; }

    /**
     * Set (override) the shortcut character.
     * In this subclass, it will change the internally stored item.
     **/
    virtual void setShortcut( char newShortcut );

protected:

    /**
     * Obtain the the shortcut property of this shortcut's widget - the string
     * that contains "&" to designate a shortcut.
     **/
    virtual std::string getShortcutString();


private:

    YItem * _item;
};


typedef std::vector<YShortcut *>	YShortcutList;
typedef YShortcutList::iterator		YShortcutListIterator;


#endif // YShortcut_h
