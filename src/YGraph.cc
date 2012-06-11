/*************************************************************************************************************

 Copyright (C) 2000 - 2010 Novell, Inc.   All Rights Reserved.

 This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 Public License as published by the Free Software Foundation; either version 2 of the License, or (at your
 option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License along with this program; if not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*************************************************************************************************************/



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

  File:		YGraph.cc

  Author:	Arvin Schnell <aschnell@suse.de>

/-*/


#define YUILogComponent "ui-graph"
#include "YUILog.h"

#include "YGraph.h"


struct YGraphPrivate
{
    YGraphPrivate( std::string filename, std::string layoutAlgorithm )
	: filename( filename ),
	  layoutAlgorithm( layoutAlgorithm )
	{}

    std::string filename;
    std::string layoutAlgorithm;
};


YGraph::YGraph( YWidget * parent, const std::string & filename, const std::string & layoutAlgorithm )
    : YWidget( parent )
    , priv( new YGraphPrivate( filename, layoutAlgorithm ) )
{
    setDefaultStretchable( YD_HORIZ, true );
    setDefaultStretchable( YD_VERT,  true );
}


YGraph::YGraph( YWidget * parent, graph_t * graph )
    : YWidget( parent )
    , priv( new YGraphPrivate( "", "" ) )
{
    setDefaultStretchable( YD_HORIZ, true );
    setDefaultStretchable( YD_VERT,  true );
}


YGraph::~YGraph()
{
    // NOP
}


std::string
YGraph::filename() const
{
    return priv->filename;
}


void
YGraph::setFilename( const std::string & filename )
{
    priv->filename = filename;
    renderGraph( filename, layoutAlgorithm() );
}


std::string
YGraph::layoutAlgorithm() const
{
    return priv->layoutAlgorithm;
}


void
YGraph::setGraph( graph_t * graph )
{
    priv->filename.clear();
    renderGraph( graph );
}


void
YGraph::setLayoutAlgorithm( const std::string & layoutAlgorithm )
{
    priv->layoutAlgorithm = layoutAlgorithm;
}


std::string
YGraph::activatedNode() const
{
    return "";
}


const YPropertySet &
YGraph::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property std::string	Filename	name of the file describing the graph
	 * @property std::string	Layout		layout-algorithm used from the graph
	 * @property std::string	Item		activated node (read-only)
	 */
	propSet.add( YProperty( YUIProperty_Filename,		YStringProperty	 ) );
	propSet.add( YProperty( YUIProperty_Layout,		YStringProperty	 ) );
	propSet.add( YProperty( YUIProperty_Item,		YStringProperty, true ) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YGraph::setProperty( const std::string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if      ( propertyName == YUIProperty_Filename	)	setFilename( val.stringVal() );
    else if ( propertyName == YUIProperty_Layout	)	setLayoutAlgorithm( val.stringVal() );
    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YGraph::getProperty( const std::string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if      ( propertyName == YUIProperty_Filename	)	return YPropertyValue( filename() );
    else if ( propertyName == YUIProperty_Layout	)	return YPropertyValue( layoutAlgorithm() );
    else if ( propertyName == YUIProperty_Item		)	return YPropertyValue( activatedNode() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}
