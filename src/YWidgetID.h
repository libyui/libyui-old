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

  File:		YWidgetID.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YWidgetID_h
#define YWidgetID_h

#include <iosfwd>
#include <string>



/**
 * Abstract base class for widget IDs.
 **/
class YWidgetID
{
protected:
    /**
     * Constructor. Protected since this is an abstract base class.
     **/
    YWidgetID() {}

public:
    /**
     * Destructor.
     **/
    virtual ~YWidgetID() {}

    /**
     * Check if this ID is equal to another.
     **/
    virtual bool isEqual( YWidgetID * otherID ) const = 0;

    /**
     * Convert the ID value to string.
     * Used for logging and debugging.
     **/
    virtual std::string toString() const = 0;

private:
    /**
     * Copy constructor is disabled.
     **/
    YWidgetID( const YWidgetID & orig );
};


/**
 * Simple widget ID class based on strings.
 **/
class YStringWidgetID: public YWidgetID
{
public:

    /**
     * Constructor.
     **/
    YStringWidgetID( const std::string & value );

    /**
     * Destructor.
     **/
    virtual ~YStringWidgetID();

    /**
     * Check if this ID is equal to another.
     *
     * Reimplemented from YWidgetID.
     **/
    virtual bool isEqual( YWidgetID * otherID ) const;

    /**
     * Convert the ID value to string.
     * Used for logging and debugging.
     *
     * Reimplemented from YWidgetID.
     **/
    virtual std::string toString() const;

    /**
     * Return the ID value.
     **/
    std::string value() const;

    /**
     * Return the ID value as a const ref.
     **/
    const std::string & valueConstRef() const;

private:
    std::string _value;
};

std::ostream & operator<<( std::ostream & stream, const YWidgetID * id );


#endif // YWidgetID_h
