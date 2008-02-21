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

  File:		YSingleChildContainerWidget.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include "YSingleChildContainerWidget.h"


YSingleChildContainerWidget::YSingleChildContainerWidget( YWidget * parent )
    : YWidget( parent )
{
    setChildrenManager( new YSingleWidgetChildManager( this ) );
}


YSingleChildContainerWidget::~YSingleChildContainerWidget()
{
    // NOP
}

	
int YSingleChildContainerWidget::preferredWidth()
{
    if ( hasChildren() )
	return firstChild()->preferredWidth();
    else
	return 0;
}


int YSingleChildContainerWidget::preferredHeight()
{
    if ( hasChildren() )
	return firstChild()->preferredHeight();
    else
	return 0;
}


void YSingleChildContainerWidget::setSize( int width, int height )
{
    if ( hasChildren() )
	firstChild()->setSize( width , height );
}


bool
YSingleChildContainerWidget::stretchable( YUIDimension dim ) const
{
    if ( hasChildren() )
	return firstChild()->stretchable( dim );
    else
	return YWidget::stretchable( dim );
}
