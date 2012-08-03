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

  File:		YShortcutManager.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui-shortcuts"
#include "YUILog.h"

#include "YShortcutManager.h"
#include "YDialog.h"
#include "YDumbTab.h"


// Threshold of widgets with valid shortcut characters below which no shortcut
// check is performed at all. This might regularly occur for languages that
// primarily use non-ASCII characters (Russian, Greek, Chinese, Japanese,
// Korean).
#define MIN_VALID_PERCENT	50

// Return the number of elements of an array of any type
#define DIM( ARRAY )	( (int) ( sizeof( ARRAY)/( sizeof( ARRAY[0] ) ) ) )


YShortcutManager::YShortcutManager( YDialog *dialog )
    : _dialog( dialog )
    , _conflictCount( 0 )
    , _didCheck( false )
{
    YUI_CHECK_PTR( _dialog );
}


YShortcutManager::~YShortcutManager()
{
    clearShortcutList();
}


void
YShortcutManager::checkShortcuts( bool autoResolve )
{
    yuiDebug() << "Checking keyboard shortcuts" << std::endl;

    clearShortcutList();
    findShortcutWidgets( _dialog->childrenBegin(), _dialog->childrenEnd() );

    int validCount = 0;

    for ( unsigned i=0; i < _shortcutList.size(); i++ )
    {
	if ( _shortcutList[i]->hasValidShortcutChar() )
	    ++validCount;
    }

    int validPercent = _shortcutList.size() > 0 ?
	( 100 * validCount ) / _shortcutList.size() : 0;

    if ( validPercent < MIN_VALID_PERCENT )
    {
        // No check at all if there are not enough widgets with valid shortcut
        // characters ([A-Za-z0-9]). This might regularly occur for languages
        // that primarily use non-ASCII characters (Russian, Greek, Chinese,
        // Japanese, Korean).

	yuiWarning() << "Not enough widgets with valid shortcut characters - no check" << std::endl;
	yuiDebug() << "Found " << validCount << " widgets with valid shortcut characters" << std::endl;
	return;
    }


    // Initialize wanted character counters
    for ( int i=0; i < DIM( _wanted ); i++ )
	_wanted[i] = 0;

    // Initialize used character flags
    for ( int i=0; i < DIM( _wanted ); i++ )
	_used[i] = false;

    // Count wanted shortcuts
    for ( unsigned i=0; i < _shortcutList.size(); i++ )
	_wanted[ (int) _shortcutList[i]->preferred() ]++;


    // Report errors

    _conflictCount = 0;

    for ( unsigned i=0; i < _shortcutList.size(); i++ )
    {
	YShortcut *shortcut = _shortcutList[i];

	if ( YShortcut::isValid( shortcut->preferred() ) )
	{
	    if ( _wanted[ (int) shortcut->preferred() ] > 1 )	// shortcut char used more than once
	    {
		shortcut->setConflict();
		_conflictCount++;

		yuiDebug() << "Shortcut conflict: '" << shortcut->preferred()
			   << "' used for " << shortcut->widget()
			   << std::endl;
	    }
	}
	else	// No or invalid shortcut
	{
	    if ( shortcut->cleanShortcutString().length() > 0 )
	    {
		shortcut->setConflict();
		_conflictCount++;

		if ( ! shortcut->widget()->autoShortcut() )
		{
		    yuiDebug() << "No valid shortcut for " << shortcut->widget() << std::endl;
		}
	    }
	}

	if ( ! shortcut->conflict() )
	{
	    _used[ (int) shortcut->preferred() ] = true;
	}
    }

    _didCheck = true;

    if ( _conflictCount > 0 )
    {
	if ( autoResolve )
	{
	    resolveAllConflicts();
	}
    }
    else
    {
	yuiDebug() << "No shortcut conflicts" << std::endl;
    }
}


void
YShortcutManager::resolveAllConflicts()
{
    yuiDebug() << "Resolving shortcut conflicts" << std::endl;

    if ( ! _didCheck )
    {
	yuiError() << "Call checkShortcuts() first!" << std::endl;
	return;
    }


    // Make a list of all shortcuts with conflicts

    YShortcutList conflictList;
    _conflictCount = 0;

    for ( YShortcutListIterator it = _shortcutList.begin();
	  it != _shortcutList.end();
	  ++it )
    {
	if ( ( *it )->conflict() )
	{
	    conflictList.push_back( *it );
	    _conflictCount++;
	}
    }


    // Resolve each conflict

    while ( ! conflictList.empty() )
    {
	//
	// Pick a conflict widget to resolve.
	//

	// Wizard buttons have priority - check any of them first.
	int prioIndex = findShortestWizardButton( conflictList );

	if ( prioIndex < 0 )
	    prioIndex = findShortestWidget( conflictList); // Find the shortest widget. Buttons have priority.


	// Pick a new shortcut for this widget.

	YShortcut * shortcut = conflictList[ prioIndex ];
	resolveConflict( shortcut );

	if ( shortcut->conflict() )
	{
	    yuiWarning() << "Couldn't resolve shortcut conflict for " << shortcut->widget() << std::endl;
	}


	// Mark this particular conflict as resolved.

	conflictList.erase( conflictList.begin() + prioIndex );
    }

    if ( _conflictCount > 0 )
    {
	yuiDebug() << _conflictCount <<  " shortcut conflict(s) left" << std::endl;
    }
}



void
YShortcutManager::resolveConflict( YShortcut * shortcut )
{
    // yuiDebug() << "Picking shortcut for " << shortcut->widget() << std::endl;

    char candidate = shortcut->preferred();			// This is always normalized, no need to normalize again.

    if ( ! YShortcut::isValid( candidate )			// Can't use this character - pick another one.
	 || _used[ (int) candidate ] )
    {
	candidate = 0;						// Restart from scratch - forget the preferred character.
	std::string str = shortcut->cleanShortcutString();

	for ( std::string::size_type pos = 0; pos < str.length(); pos++ )	// Search all the shortcut string.
	{
	    char c = YShortcut::normalized( str[ pos ] );
	    // yuiDebug() << "Checking '" << c << "'" << std::endl;

	    if ( YShortcut::isValid(c) && ! _used[ (int) c ] ) 	// Could we use this character?
	    {
		if ( _wanted[ (int) c ] < _wanted[ (int) candidate ]	// Is this a better choice than what we already have -
		     || ! YShortcut::isValid( candidate ) )		// or don't we have anything yet?
		{
		    candidate = c;			// Use this one.
		    // yuiDebug() << "Picking '" << c << "'" << std::endl;

		    if ( _wanted[ (int) c ] == 0 )	// It doesn't get any better than this:
			break;				// Nobody wants this shortcut anyway.
		}
	    }
	}
    }

    if ( YShortcut::isValid( candidate ) )
    {
	if ( candidate != shortcut->preferred() )
	{
	    if ( shortcut->widget()->autoShortcut() )
	    {
		yuiDebug() << "Automatically assigning shortcut '" << candidate
			   << "' to " << shortcut->widgetClass() << "(`opt(`autoShortcut ), \""
			   << shortcut->cleanShortcutString() << "\" )"
			   << std::endl;
	    }
	    else
	    {
		yuiDebug() << "Reassigning shortcut '" << candidate
			   << "' to " << shortcut->widget()
			   << std::endl;
	    }
	    shortcut->setShortcut( candidate );
	}
	else
	{
	    yuiDebug() << "Keeping preferred shortcut '" << candidate
		       << "' for " << shortcut->widget()
		       << std::endl;
	}

	_used[ (int) candidate ] = true;
	shortcut->setConflict( false );
    }
    else	// No unique shortcut found
    {
	yuiWarning() << "Couldn't resolve shortcut conflict for "
		     << shortcut->widget()
		     << " - assigning no shortcut"
		     << std::endl;

	shortcut->clearShortcut();
	shortcut->setConflict( false );
    }

    _conflictCount--;
}



int
YShortcutManager::findShortestWizardButton( const YShortcutList & conflictList )
{
    int shortestIndex = -1;
    int shortestLen   = -1;

    for ( unsigned i=1; i < conflictList.size(); i++ )
    {
	if ( conflictList[i]->isWizardButton() )
	{
	    if ( shortestLen < 0 ||
		 conflictList[i]->distinctShortcutChars() < shortestLen )
	    {
		shortestIndex = i;
		shortestLen   = conflictList[i]->distinctShortcutChars();
	    }

	}
    }

    return shortestIndex;
}



unsigned
YShortcutManager::findShortestWidget( const YShortcutList & conflictList )
{
    unsigned shortestIndex = 0;
    int      shortestLen   = conflictList[ shortestIndex ]->distinctShortcutChars();

    for ( unsigned i=1; i < conflictList.size(); i++ )
    {
	int currentLen = conflictList[i]->distinctShortcutChars();

	if ( currentLen < shortestLen )
	{
	    // Found an even shorter one

	    shortestIndex = i;
	    shortestLen   = currentLen;
	}
	else if ( currentLen == shortestLen )
	{
	    if ( conflictList[i]->isButton() &&
		 ! conflictList[ shortestIndex ]->isButton() )
	    {
		// Prefer a button over another widget with the same length

		shortestIndex = i;
		shortestLen   = currentLen;
	    }
	}
    }

    return shortestIndex;
}



void
YShortcutManager::clearShortcutList()
{
    for ( unsigned i=0; i < _shortcutList.size(); i++ )
    {
	delete _shortcutList[i];
    }

    _shortcutList.clear();
}


void
YShortcutManager::findShortcutWidgets( YWidgetListConstIterator begin,
				       YWidgetListConstIterator end )
{
    for ( YWidgetListConstIterator it = begin; it != end; ++it )
    {
	YWidget * widget = *it;

	YDumbTab * dumbTab = dynamic_cast<YDumbTab *> (widget);

	if ( dumbTab )
	{
	    for ( YItemConstIterator it = dumbTab->itemsBegin();
		  it != dumbTab->itemsEnd();
		  ++it )
	    {
		YItemShortcut * shortcut = new YItemShortcut( dumbTab, *it );
		_shortcutList.push_back( shortcut );
	    }
	}
	else if ( ! widget->shortcutString().empty() )
	{
	    YShortcut * shortcut = new YShortcut( *it );
	    _shortcutList.push_back( shortcut );
	}

	if ( widget->hasChildren() )
	{
	    findShortcutWidgets( widget->childrenBegin(),
				 widget->childrenEnd()   );
	}
    }
}
