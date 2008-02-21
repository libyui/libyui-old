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

  File:		YWidgetID.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YWidgetID_h
#define YWidgetID_h

#include <iosfwd>
#include <string>

using std::string;


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
    virtual string toString() const = 0;

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
    YStringWidgetID( const string & value );

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
    virtual string toString() const;

    /**
     * Return the ID value.
     **/
    string value() const;

    /**
     * Return the ID value as a const ref.
     **/
    const string & valueConstRef() const;

private:
    string _value;
};

std::ostream & operator<<( std::ostream & stream, const YWidgetID * id );


#endif // YWidgetID_h
