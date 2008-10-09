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

  File:		YEventFilter.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include "YEventFilter.h"
#include "YEvent.h"
#include "YDialog.h"
#include "YUIException.h"


struct YEventFilterPrivate
{
    YEventFilterPrivate( YDialog * dialog )
	: dialog( dialog )
	{}
    
    YDialog * dialog;
};




YEventFilter::YEventFilter( YDialog * dialog )
    : priv( new YEventFilterPrivate( dialog ) )
{
    YUI_CHECK_NEW( priv );

    if ( ! dialog )
	priv->dialog = YDialog::currentDialog(); // throw if no dialog

    priv->dialog->addEventFilter( this );
}


YEventFilter::~YEventFilter()
{
    priv->dialog->removeEventFilter( this );
}


YDialog *
YEventFilter::dialog() const
{
    return priv->dialog;
}
