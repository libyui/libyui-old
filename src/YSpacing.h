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

  File:		YSpacing.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YSpacing_h
#define YSpacing_h

#include "YWidget.h"
#include "ImplPtr.h"

class YSpacingPrivate;


/**
 * HSpacing, VSpacing, HStretch, VStretch
 **/
class YSpacing: public YWidget
{
public:

    /**
     * Constructor.
     *
     * A Spacing/Stretch widget works only in one dimension ('dim') at the same
     * time. But it can be stretchable and have a size at the same time, in
     * which case the specified size acts very much like a minimal size - but
     * not exactly, since YLayoutBox will reduce Spacings first before other
     * widgets have to be resized below their preferred size.
     *
     * 'layoutUnits' is specified in abstract UI units where a main window
     * (800x600 pixels in the Qt UI) corresponds to a 80x25 window.
     **/
    YSpacing( YWidget *		parent,
	      YUIDimension 	dim,
	      bool 		stretchable	= false,
	      YLayoutSize_t 	layoutUnits	= 0.0 );

    /**
     * Destructor.
     **/
    virtual ~YSpacing();
    
    /**
     * Return a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YSpacing"; }

    /**
     * Return the primary dimension of this Spacing/Stretch,
     * i.e. the dimension in which it uses space or stretches.
     **/
    YUIDimension dimension() const;

    /**
     * Return the size in the primary dimension.
     *
     * This is the device dependent size (pixels or character cells), not the
     * abstract UI layout unit from the constructor.
     **/
    int size() const;

    /**
     * Return the size in the specified dimension.
     *
     * This is the device dependent size (pixels or character cells), not the
     * abstract UI layout unit from the constructor.
     **/
    int size( YUIDimension dim ) const;

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

    
private:

    ImplPtr<YSpacingPrivate> priv;
};


typedef YSpacing YVSpacing;
typedef YSpacing YHSpacing;
typedef YSpacing YHStretch;
typedef YSpacing YVStretch;


#endif // YSpacing_h
