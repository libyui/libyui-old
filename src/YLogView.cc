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



 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////   __/\\\\\\_____________/\\\__________/\\\________/\\\___/\\\________/\\\___/\\\\\\\\\\\_           ////
 ////    _\////\\\____________\/\\\_________\///\\\____/\\\/___\/\\\_______\/\\\__\/////\\\///__          ////
 ////     ____\/\\\______/\\\__\/\\\___________\///\\\/\\\/_____\/\\\_______\/\\\______\/\\\_____         ////
 ////      ____\/\\\_____\///___\/\\\_____________\///\\\/_______\/\\\_______\/\\\______\/\\\_____        ////
 ////       ____\/\\\______/\\\__\/\\\\\\\\\_________\/\\\________\/\\\_______\/\\\______\/\\\_____       ////
 ////        ____\/\\\_____\/\\\__\/\\\////\\\________\/\\\________\/\\\_______\/\\\______\/\\\_____      ////
 ////         ____\/\\\_____\/\\\__\/\\\__\/\\\________\/\\\________\//\\\______/\\\_______\/\\\_____     ////
 ////          __/\\\\\\\\\__\/\\\__\/\\\\\\\\\_________\/\\\_________\///\\\\\\\\\/_____/\\\\\\\\\\\_    ////
 ////           _\/////////___\///___\/////////__________\///____________\/////////______\///////////__   ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                 widget abstraction library providing Qt, GTK and ncurses frontends                  ////
 ////                                                                                                     ////
 ////                                   3 UIs for the price of one code                                   ////
 ////                                                                                                     ////
 ////                                       ***  core library  ***                                        ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                              (C) SUSE Linux GmbH    ////
 ////                                                                                                     ////
 ////                                                              libYUI-AsciiArt (C) 2012 Björn Esser   ////
 ////                                                                                                     ////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-/

  File:         YLogView.cc

  Author:       Stefan Hundhammer <sh@suse.de>

/-*/

#include <deque>

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YLogView.h"


typedef std::deque<std::string>                      StringDeque;
typedef std::deque<std::string>::iterator            StringDequeIterator;
typedef std::deque<std::string>::const_iterator      StringDequeConstIterator;



struct YLogViewPrivate
{
    YLogViewPrivate( const std::string & label, int visibleLines, int maxLines )
        : label( label )
        , visibleLines( visibleLines )
        , maxLines( maxLines )
        {}

    std::string	label;
    int		visibleLines;
    int		maxLines;

    StringDeque logText;
};




YLogView::YLogView( YWidget * parent, const std::string & label, int visibleLines, int maxLines )
    : YWidget( parent )
    , priv( new YLogViewPrivate( label, visibleLines, maxLines ) )
{
    YUI_CHECK_NEW( priv );

    setDefaultStretchable( YD_HORIZ, true );
    setDefaultStretchable( YD_VERT,  true );
}


YLogView::~YLogView()
{
    // NOP
}


std::string
YLogView::label() const
{
    return priv->label;
}


void
YLogView::setLabel( const std::string & label )
{
    priv->label = label;
}


int
YLogView::visibleLines() const
{
    return priv->visibleLines;
}


void
YLogView::setVisibleLines( int newVisibleLines )
{
    priv->visibleLines = newVisibleLines;
}


int
YLogView::maxLines() const
{
    return priv->maxLines;
}


void
YLogView::setMaxLines( int newMaxLines )
{
    int linesToDelete = priv->maxLines - newMaxLines;
    priv->maxLines = newMaxLines;

    for ( int i=0; i < linesToDelete; i++ )
        priv->logText.pop_front();

    if ( linesToDelete > 0 )
	updateDisplay();
}


std::string
YLogView::logText() const
{
    std::string text;

    for ( StringDequeConstIterator it = priv->logText.begin();
          it != priv->logText.end();
          ++it )
    {
        text += *it;
    }

    if ( ! text.empty() )
    {
	// Cut off last newline

	if ( *(text.rbegin()) == '\n' ) // Last char is a newline?
	{
	    text.resize( text.size() - 1 ); // make one char shorter
	}
    }

    return text;
}


std::string
YLogView::lastLine() const
{
    if ( priv->logText.empty() )
        return "";
    else
        return priv->logText.back();
}


void
YLogView::appendLines( const std::string & newText )
{
    std::string			text	= newText;
    std::string::size_type	from	= 0;
    std::string::size_type	to	= 0;


    // Split the text into single lines

    while ( to < text.size() )
    {
        from = to;
        to   = text.find( '\n', from );
        if ( to == std::string::npos )               // no more newline
            to = text.size();
        else
            to++;                               // include the newline

        // Output one single line
        appendLine( text.substr( from, to - from ) );
    }

    if ( to < text.size() )             // anything left over?
    {
        // Output the rest
        appendLine( text.substr( to, text.size() - to ) );
    }

    updateDisplay();
}


void
YLogView::appendLine( const std::string & line )
{
    priv->logText.push_back( line );

    if ( maxLines() > 0 && priv->logText.size() > (unsigned) maxLines() )
    {
        priv->logText.pop_front();
    }
}


void
YLogView::clearText()
{
    priv->logText.clear();
    updateDisplay();
}


int YLogView::lines() const
{
    return priv->logText.size();
}


void
YLogView::updateDisplay()
{
    displayLogText( logText() );
}



const YPropertySet &
YLogView::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
        /*
         * @property std::string	Value		All log lines.
         * @property std::string	LastLine	The last log line(s). Use this to append lines.
         * @property integer		VisibleLines	Number of lines to display. Call RecalcLayout() afterwards.
         * @property integer		MaxLines	Number of lines to store (0 for all).
         * @property std::string	Label		Caption above the log text
         */
        propSet.add( YProperty( YUIProperty_Value,              YStringProperty ) );
        propSet.add( YProperty( YUIProperty_LastLine,           YStringProperty ) );
        propSet.add( YProperty( YUIProperty_VisibleLines,       YIntegerProperty ) );
        propSet.add( YProperty( YUIProperty_MaxLines,           YIntegerProperty ) );
        propSet.add( YProperty( YUIProperty_Label,              YStringProperty  ) );
        propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YLogView::setProperty( const std::string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if      ( propertyName == YUIProperty_Value         )       setLogText	( val.stringVal()  );
    else if ( propertyName == YUIProperty_LastLine      )       appendLines     ( val.stringVal()  );
    else if ( propertyName == YUIProperty_VisibleLines  )       setVisibleLines ( val.integerVal() );
    else if ( propertyName == YUIProperty_MaxLines      )       setMaxLines     ( val.integerVal() );
    else if ( propertyName == YUIProperty_Label         )       setLabel        ( val.stringVal()  );
    else
    {
        return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YLogView::getProperty( const std::string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if      ( propertyName == YUIProperty_Value         )       return YPropertyValue( logText()      );
    if      ( propertyName == YUIProperty_LastLine      )       return YPropertyValue( lastLine()     );
    if      ( propertyName == YUIProperty_VisibleLines  )       return YPropertyValue( visibleLines() );
    if      ( propertyName == YUIProperty_MaxLines      )       return YPropertyValue( maxLines()     );
    else if ( propertyName == YUIProperty_Label         )       return YPropertyValue( label()        );
    else
    {
        return YWidget::getProperty( propertyName );
    }
}
