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

  File:		YTableHeader.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <vector>

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YTableHeader.h"
#include "YUIException.h"



struct YTableHeaderPrivate
{
    YTableHeaderPrivate()
	{}

    std::vector<std::string>	headers;
    std::vector<YAlignmentType>	alignments;
};




YTableHeader::YTableHeader()
    : priv( new YTableHeaderPrivate )
{
    YUI_CHECK_NEW( priv );
}


YTableHeader::~YTableHeader()
{
    // NOP
}


void
YTableHeader::addColumn( const std::string & header, YAlignmentType alignment )
{
    priv->headers.push_back( header );
    priv->alignments.push_back( alignment );
}


int
YTableHeader::columns() const
{
    return (int) priv->headers.size();
}


bool
YTableHeader::hasColumn( int column ) const
{
    return column >= 0 && column < (int) priv->headers.size();
}


std::string
YTableHeader::header( int column ) const
{
    if ( column >= 0 && column < (int) priv->headers.size() )
	return priv->headers[ column ];
    else
	return "";
}


YAlignmentType
YTableHeader::alignment( int column ) const
{
    if ( column >= 0 && column < (int) priv->alignments.size() )
	return priv->alignments[ column ];
    else
	return YAlignBegin;
}
