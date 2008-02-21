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

  File:		YTimezoneSelector.cc

  Author:	Stephan Kulow <coolo@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YTimezoneSelector.h"


class YTimezoneSelectorPrivate
{
   bool dummy;
};




YTimezoneSelector::YTimezoneSelector( YWidget *		parent,
                                      const string &pixmap,
                                      const map<string, string> &timezones )
    : YWidget( parent )
{
}


YTimezoneSelector::~YTimezoneSelector()
{
    // NOP
}


const YPropertySet &
YTimezoneSelector::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
        propSet.add( YProperty( YUIProperty_Value, YStringProperty ) );
        propSet.add( YProperty( YUIProperty_CurrentItem, YStringProperty ) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YTimezoneSelector::setProperty( const string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if ( propertyName == YUIProperty_Value )
    {
        setCurrentZone( val.stringVal(), true );
        return true; // success -- no special handling necessary
    }
    if ( propertyName == YUIProperty_CurrentItem )
    {
        setCurrentZone( val.stringVal(), false );
        return true; // success -- no special handling necessary
    }
    return YWidget::setProperty( propertyName, val );
}


YPropertyValue
YTimezoneSelector::getProperty( const string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if ( propertyName == YUIProperty_Value ) 	return YPropertyValue( currentZone() );
    if ( propertyName == YUIProperty_CurrentItem )    return YPropertyValue( currentZone() );
    return YWidget::getProperty( propertyName );
}

