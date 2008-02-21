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

  File:		YSingleChildContainerWidget.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YSingleChildContainerWidget_h
#define YSingleChildContainerWidget_h

#include "YWidget.h"

using std::string;

/**
 * Container widget class that manages one child.
 **/
class YSingleChildContainerWidget: public YWidget
{
protected:
    /**
     * Constructor.
     **/
    YSingleChildContainerWidget( YWidget * parent );

public:
    /**
     * Destructor.
     **/
    virtual ~YSingleChildContainerWidget();

    /**
     * Preferred width of the widget.
     *
     * Reimplemented from YWidget.
     **/
    virtual int preferredWidth();

    /**
     * Preferred height of the widget.
     *
     * Reimplemented from YWidget.
     **/
    virtual int preferredHeight();
    
    /**
     * Set the new size of the widget.
     * In this case, the size of the single child is set.
     *
     * Reimplemented from YWidget.
     **/
    virtual void setSize( int newWidth, int newHeight );

    /**
     * Returns 'true' if this widget is stretchable in the specified dimension.
     * In this case, the stretchability of the single child is returned.
     *
     * Reimplemented from YWidget.
     **/
    virtual bool stretchable( YUIDimension dim ) const;
};



#endif // YSingleChildContainerWidget_h
