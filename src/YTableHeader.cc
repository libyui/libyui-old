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

  File:		YTableHeader.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <vector>

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YTableHeader.h"
#include "YUIException.h"

using std::vector;


struct YTableHeaderPrivate
{
    YTableHeaderPrivate()
	{}

    vector<string>		headers;
    vector<YAlignmentType>	alignments;
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
YTableHeader::addColumn( const string & header, YAlignmentType alignment )
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


string
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

