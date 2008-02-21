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

  File:		YItem.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include "YItem.h"

/**
 * Static children collection that is always empty so the children
 * iterators of this base class have something valid to return.
 *
 * No item will ever be added to this collection.
 **/
YItemCollection YItem::_noChildren;

