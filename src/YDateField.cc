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

  File:		YDateField.cc

  Author:     	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YDateField.h"


struct YDateFieldPrivate
{
    YDateFieldPrivate()
	{}

    bool dummy;
};




YDateField::YDateField( YWidget * parent, const string & label )
    : YSimpleInputField( parent, label )
    , priv( new YDateFieldPrivate() )
{
    YUI_CHECK_NEW( priv );
}


YDateField::~YDateField()
{
    // NOP
}


/*
 * Properties (all handled in YSimpleInputField):
 *
 * @property string  Value	the date (the field's contents) as "YYYY-MM-DD"
 * @property string  Label	caption above the input field
 */

