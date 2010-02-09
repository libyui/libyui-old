/**************************************************************************
Copyright (C) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************/


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
