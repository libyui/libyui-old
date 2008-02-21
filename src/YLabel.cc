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

  File:		YLabel.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#define MAX_DEBUG_LABEL_LEN	32


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YLabel.h"


struct YLabelPrivate
{
    /**
     * Constructor
     **/
    YLabelPrivate( const string &	text,
		   bool 		isHeading,
		   bool 		isOutputField )
	: text( text )
	, isHeading( isHeading )
	, isOutputField( isOutputField )
	, useBoldFont( false )
	{}
    
    string	text;
    bool	isHeading;
    bool	isOutputField;
    bool	useBoldFont;
};


YLabel::YLabel( YWidget * 	parent,
		const string &	text,
		bool 		isHeading,
		bool 		isOutputField )
    : YWidget( parent )
    , priv( new YLabelPrivate( text, isHeading, isOutputField ) )
{
    YUI_CHECK_NEW( priv );
}


YLabel::~YLabel()
{
    // NOP
}


string YLabel::text() const
{
    return priv->text;
}


void YLabel::setText( const string & newText )
{
    priv->text = newText;
}


bool YLabel::isHeading() const
{
    return priv->isHeading;
}


bool YLabel::isOutputField() const
{
    return priv->isOutputField;
}


bool YLabel::useBoldFont() const
{
    return priv->useBoldFont;
}


void YLabel::setUseBoldFont( bool bold )
{
    priv->useBoldFont = bold;
}


const YPropertySet &
YLabel::propertySet()
{
    static YPropertySet propSet;
    
    if ( propSet.isEmpty() )
    {
	/*
	 * @property string Label	the label text
	 * @property string Value 	the label text (alias for Label)
	 * @property string Text	the label text (alias for Label)
	 */
	
	propSet.add( YProperty( YUIProperty_Label,	YStringProperty	) );
	propSet.add( YProperty( YUIProperty_Value,	YStringProperty	) );
	propSet.add( YProperty( YUIProperty_Text,	YStringProperty	) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YLabel::setProperty( const string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if	    ( propertyName == YUIProperty_Label )	setText( val.stringVal() );
    else if ( propertyName == YUIProperty_Value )	setText( val.stringVal() );
    else if ( propertyName == YUIProperty_Text )	setText( val.stringVal() );
    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YLabel::getProperty( const string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if      ( propertyName == YUIProperty_Label )	return YPropertyValue( text() );
    else if ( propertyName == YUIProperty_Value )	return YPropertyValue( text() );
    else if ( propertyName == YUIProperty_Text )	return YPropertyValue( text() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}


string YLabel::debugLabel() const
{
    string label = text();
    
    if ( label.size() > MAX_DEBUG_LABEL_LEN )
    {
	label.resize( MAX_DEBUG_LABEL_LEN );
	label.append( "..." );
    }

    for ( string::size_type i=0; i < label.size(); i++ )
    {
	if ( label[i] == '\n' )		label[i] = ' ';
	if ( label[i] == '\"' )		label[i] = ' ';
    }

    return label;
}
