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

  File:		YBothDim.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YBothDim_h
#define YBothDim_h

#include "YTypes.h"
#include "YUIException.h"


/**
 * Template class for two-dimensional entities, such as
 * - width, height
 * - x_pos, y_pos
 * - hStretchable, vStretchable
 *
 * Precondition: type T needs to have a default constructor
 * (which all simple types like int, long, bool have).
 **/
template<typename T> class YBothDim
{
public:

    // Data Members - intentionally public
    T vert;
    T hor;

    /**
     * Constructor with explicit initialization for both values
     **/
    YBothDim( T hor, T vert )
	: vert( vert )
	, hor( hor )
	{}

    /**
     * Default constructor (calls T default constructor for both values)
     **/
    YBothDim()
	{}

    /**
     * operator[] for alternative access via  myVar[ YD_HORIZ ]
     * Please note that this returns a non-const reference, so this can be used
     * as an lvalue (e.g., in assignments) 
     **/
    T & operator[]( YUIDimension dim )
    {
	switch ( dim )
	{
	    case YD_HORIZ:	return hor;
	    case YD_VERT:	return vert;
	    default:		YUI_THROW( YUIInvalidDimensionException() );
	}
	
	// never reached (but gcc will complain otherwise)
	return hor; 
    }

    /**
     * Same as above for const objects
     **/
    const T & operator[]( YUIDimension dim ) const
    {
	switch ( dim )
	{
	    case YD_HORIZ:	return hor;
	    case YD_VERT:	return vert;
	    default:		YUI_THROW( YUIInvalidDimensionException() );
	}
	
	// never reached (but gcc will complain otherwise)
	return hor; 
    }
};


#endif // YBothDim_h
