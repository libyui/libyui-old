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

  File:		YTableHeader.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YTableHeader_h
#define YTableHeader_h

#include <string>
#include "ImplPtr.h"
#include "YTypes.h"



class YTableHeaderPrivate;

/**
 * Helper class for YTable for table column properties:
 *
 *   - number of columns
 *   - header    for each column
 *   - alignment for each column
 **/
class YTableHeader
{
public:
    /**
     * Constructor.
     **/
    YTableHeader();

    /**
     * Destructor.
     **/
    virtual ~YTableHeader();

    /**
     * Add a column with the specified colum header text and alignment.
     **/
    void addColumn( const std::string &	header,
		    YAlignmentType	alignment = YAlignBegin );

    /**
     * Return the number of columns.
     **/
    int columns() const;

    /**
     * Return 'true' if this table header has a column no. 'column'
     * (counting from 0 on).
     **/
    bool hasColumn( int column ) const;

    /**
     * Return the header text for the specified column.
     **/
    std::string header( int column ) const;

    /**
     * Return the alignment for the specified column.
     **/
    YAlignmentType alignment( int column ) const;


private:

    ImplPtr<YTableHeaderPrivate> priv;
};


#endif // YTableHeader_h
