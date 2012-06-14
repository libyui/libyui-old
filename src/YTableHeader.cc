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
