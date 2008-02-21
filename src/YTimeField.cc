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

  File:		YTimeField.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YTimeField.h"


struct YTimeFieldPrivate
{
    YTimeFieldPrivate()
	{}

    bool dummy;
};




YTimeField::YTimeField( YWidget * parent, const string & label )
    : YSimpleInputField( parent, label )
    , priv( new YTimeFieldPrivate() )
{
    YUI_CHECK_NEW( priv );
}


YTimeField::~YTimeField()
{
    // NOP
}


/*
 * Properties (all handled in YSimpleInputField):
 *
 * @property string  Value	the time (the field's contents) as "hh:mm:ss"
 * @property string  Label	caption above the input field
 */
