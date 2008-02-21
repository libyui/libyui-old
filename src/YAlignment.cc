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

  File:		YAlignment.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YAlignment.h"
#include "YBothDim.h"


using std::min;
using std::max;


struct YAlignmentPrivate
{
    /**
     * Constructor.
     **/
    YAlignmentPrivate( YAlignmentType horAlign,
		       YAlignmentType vertAlign )
	: leftMargin(0)
	, rightMargin(0)
	, topMargin(0)
	, bottomMargin(0)
	, minWidth(0)
	, minHeight(0)
	{
	    alignment.hor  = horAlign;
	    alignment.vert = vertAlign;
	}

    //
    // Data Members
    //

    int leftMargin;
    int rightMargin;
    int topMargin;
    int bottomMargin;

    int minWidth;
    int minHeight;

    string backgroundPixmap;

    YBothDim<YAlignmentType> alignment;
};




YAlignment::YAlignment( YWidget * 	parent,
			YAlignmentType 	horAlign,
			YAlignmentType 	vertAlign )
    : YSingleChildContainerWidget( parent )
    , priv( new YAlignmentPrivate( horAlign, vertAlign ) )
{
    YUI_CHECK_NEW( priv );
}


YAlignment::~YAlignment()
{
    // NOP
}


YAlignmentType
YAlignment::alignment( YUIDimension dim ) const
{
    return priv->alignment[ dim ];
}


int YAlignment::leftMargin() const
{
    return priv->leftMargin;
}


int YAlignment::rightMargin() const
{
    return priv->rightMargin;
}


int YAlignment::topMargin() const
{
    return priv->topMargin;
}


int YAlignment::bottomMargin() const
{
    return priv->bottomMargin;
}


void YAlignment::setLeftMargin( int margin )
{
    priv->leftMargin = margin;
}


void YAlignment::setRightMargin( int margin )
{
    priv->rightMargin = margin;
}


void YAlignment::setTopMargin( int margin )
{
    priv->topMargin = margin;
}


void YAlignment::setBottomMargin( int margin )
{
    priv->bottomMargin = margin;
}


int YAlignment::minWidth() const
{
    return priv->minWidth;
}


int YAlignment::minHeight() const
{
    return priv->minHeight;
}


void YAlignment::setMinWidth( int width )
{
    priv->minWidth = width;
}


void YAlignment::setMinHeight( int height )
{
    priv->minHeight = height;
}


string YAlignment::backgroundPixmap() const
{
    return priv->backgroundPixmap;
}


void YAlignment::addChild( YWidget * child )
{
    YSingleChildContainerWidget::addChild( child );
    
    if ( minWidth()  > 0 )	child->setStretchable( YD_HORIZ, true );
    if ( minHeight() > 0 )	child->setStretchable( YD_VERT , true );
}


bool YAlignment::stretchable( YUIDimension dim ) const
{
    if ( alignment( dim ) == YAlignUnchanged && hasChildren() )
	return firstChild()->stretchable( dim );
    else
	return true;
}


int YAlignment::preferredWidth()
{
    if ( ! hasChildren() )
	return minWidth();

    int preferredWidth = firstChild()->preferredWidth();
    preferredWidth    += leftMargin() + rightMargin();
	
    return max( minWidth(), preferredWidth );
}


int YAlignment::preferredHeight()
{
    if ( ! hasChildren() )
	return minHeight();

    int preferredHeight = firstChild()->preferredHeight();
    preferredHeight    += topMargin() + bottomMargin();
	
    return max( minHeight(), preferredHeight );
}


void YAlignment::setSize( int newWidth, int newHeight )
{
    if ( ! hasChildren() )
    {
	yuiError() << "No child in " << this << endl;
	return;
    }


    YBothDim<int> newSize;
    newSize.hor  = newWidth;
    newSize.vert = newHeight;

    YBothDim<int> offset;
    offset.hor  = leftMargin();
    offset.vert = topMargin();

    YBothDim<int> totalMargin;
    totalMargin.hor  = leftMargin() + rightMargin();
    totalMargin.vert = topMargin()  + bottomMargin();

    YBothDim<int> newChildSize;
    YBothDim<int> newChildPos;


    for ( YUIDimension dim = YD_HORIZ; dim <= YD_VERT; dim = (YUIDimension) (dim+1) )
    {
	int childPreferredSize = firstChild()->preferredSize( dim );
	int preferredSize      = childPreferredSize + totalMargin[ dim ];

	if ( newSize[ dim ] >= preferredSize )
	    // Optimum case: enough space for the child and all margins
	{
	    if ( firstChild()->stretchable( dim ) &&
		 ( alignment( dim ) == YAlignUnchanged ||
		   stretchable( dim ) ) )	// special case: promote child stretchability if `opt(`?stretch) set
	    {
		newChildSize[ dim ] = newSize[ dim ] - totalMargin[ dim ];
	    }
	    else
	    {
		newChildSize[ dim ] = childPreferredSize;
	    }
	}
	else if ( newSize[ dim ] >= childPreferredSize )
	    // Still enough space for the child, but not for all margins
	{
	    newChildSize[ dim ] = childPreferredSize; // Give the child as much space as it needs

	    // Reduce the margins

	    if ( totalMargin[ dim ] > 0 ) // Prevent division by zero
	    {
		// Redistribute remaining space according to margin ratio
		// (disregarding integer rounding errors - we don't care about one pixel)

		int remaining = newSize[ dim ] - childPreferredSize;
		offset     [ dim ] = remaining * offset[ dim ] / totalMargin[ dim ];
		totalMargin[ dim ] = remaining;
	    }

	}
	else // Not even enough space for the child - forget about the margins
	{
	    newChildSize[ dim ] = newSize[ dim ];
	    offset	[ dim ] = 0;
	    totalMargin [ dim ] = 0;
	}


	switch ( alignment( dim ) )
	{
	    case YAlignCenter:
		newChildPos[ dim ] = ( newSize[ dim ] - newChildSize[ dim ] - totalMargin[ dim ] ) / 2;
		break;

	    case YAlignUnchanged:
	    case YAlignBegin:
		newChildPos[ dim ] = 0;
		break;

	    case YAlignEnd:
		newChildPos[ dim ] = newSize[ dim ] - newChildSize[ dim ] - totalMargin[ dim ];
		break;
	}

	newChildPos[ dim ] += offset[ dim ];
    }

    firstChild()->setSize( newChildSize.hor, newChildSize.vert );
    moveChild( firstChild(), newChildPos.hor, newChildPos.vert );

#if 0
    yuiDebug() << "setSize( alignment, " << newWidth         << ", " << newHeight         << ")" << endl;
    yuiDebug() << "setSize( child, "     << newChildSize.hor << ", " << newChildSize.vert << ")" << endl;
    yuiDebug() << "moveChild( "          << newChildPos.hor  << ", " << newChildPos.vert  << ")" << endl;
#endif
}



int YAlignment::totalMargins( YUIDimension dim ) const
{
    if ( dim == YD_HORIZ )	return leftMargin() + rightMargin();
    else			return topMargin()  + bottomMargin();
}



void YAlignment::setBackgroundPixmap( const string & pixmapFileName )
{
    string pixmap = pixmapFileName;
    
    if ( pixmap.length() > 0 &&
	 pixmap[0] != '/'  &&	// Absolute path?
	 pixmap[0] != '.'    )	// Path relative to $CWD ?
    {
	// Prepend theme dir ("/usr/share/YaST2/theme/current/")
	pixmap = pixmap.insert( 0, string( THEMEDIR "/" ) );
    }

    priv->backgroundPixmap = pixmap;
}
