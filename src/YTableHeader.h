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

  File:		YTableHeader.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YTableHeader_h
#define YTableHeader_h

#include <string>
#include "ImplPtr.h"
#include "YTypes.h"

using std::string;


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
    void addColumn( const string &	header,
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
    string header( int column ) const;

    /**
     * Return the alignment for the specified column.
     **/
    YAlignmentType alignment( int column ) const;


private:
    
    ImplPtr<YTableHeaderPrivate> priv;
};


#endif // YTableHeader_h
