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

  File:		YUIException.cc

		Stolen from zypp/libzypp/base/Exception.cc

  Author:	Michael Andres    <ma@suse.de>
  Maintainer:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <sstream>
#include <string.h>	// strerror()
#include <stdio.h>

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUIException.h"
#include "YWidget.h"


using std::string;
using std::ostream;
using std::endl;


string YCodeLocation::asString() const
{
    string str( _file );
    str += "(" + _func + "):";

    char formatted_number[ 20 ];
    sprintf( formatted_number, "%u", _line );

    str += formatted_number;

    return str;
}


ostream &
operator<<( ostream & str, const YCodeLocation & obj )
{
    return str << obj.asString();
}


YUIException::YUIException()
{
    // NOP
}

YUIException::YUIException( const string & msg_r )
    : _msg( msg_r )
{
    // NOP
}


YUIException::~YUIException() throw()
{
    // NOP
}


string
YUIException::asString() const
{
    std::ostringstream str;
    dumpOn( str );
    return str.str();
}


ostream &
YUIException::dumpOn( ostream & str ) const
{
    return str << _msg;
}


ostream &
YUIException::dumpError( ostream & str ) const
{
    return dumpOn( str << _where << ": " );
}


ostream &
operator<<( ostream & str, const YUIException & obj )
{
    return obj.dumpError( str );
}


string
YUIException::strErrno( int errno_r )
{
    return strerror( errno_r );
}


string
YUIException::strErrno( int errno_r, const string & msg )
{
    string ret( msg );
    ret += ": ";
    return ret += strErrno( errno_r );
}


void
YUIException::log( const YUIException & 	exception,
		   const YCodeLocation & 	location,
		   const char * const 		prefix )
{
    YUILog::warning( YUILogComponent,
		     location.file().c_str(),
		     location.line(),
		     location.func().c_str() )
			 << "\t" << prefix << " " << exception.asString() << endl;
}


ostream &
YUIUnknownPropertyException::dumpOn( ostream & str ) const
{
    if ( widget() )
    {
	return str << widget()->widgetClass()
		   << " has no property named \""
		   << property().name()
		   << "\""
		   << endl;
    }
    else
    {
	return str << "Unknown property name \""
		   << property().name()
		   << "\""
		   << endl;
    }
}


ostream &
YUIPropertyTypeMismatchException::dumpOn( ostream & str ) const
{
    std::string widgetClass;

    if ( widget() )
	widgetClass = std::string( widget()->widgetClass() ) + "::";

    return str << "Property type mismatch: "
	       << widgetClass
	       << property().name()
	       << " is type "
	       << property().typeAsStr()
	       << ", not "
	       << YProperty::typeAsStr( type() )
	       << endl;
}


ostream &
YUISetReadOnlyPropertyException::dumpOn( ostream & str ) const
{
    std::string widgetClass;

    if ( widget() )
	widgetClass = std::string( widget()->widgetClass() ) + "::";

    return str << "Property "
	       << widgetClass
	       << property().name()
	       << "is read-only!"
	       << endl;
}


ostream &
YUIBadPropertyArgException::dumpOn( ostream & str ) const
{
    std::string widgetClass;

    if ( widget() )
	widgetClass = std::string( widget()->widgetClass() ) + "::";

    return str << "Bad argument for property "
	       << widgetClass
	       << property().name()
	       << ": "
	       << msg()
	       << endl;
}


