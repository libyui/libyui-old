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

  File:		YImage.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YImage_h
#define YImage_h

#include "YWidget.h"
#include <string>

using std::string;

class YImagePrivate;


class YImage : public YWidget
{
public:
    /**
     * Constructor.
     *
     * 'animated' indicates if 'imageFileName' is an animated image format
     * (e.g., MNG).
     **/
    YImage( YWidget *		parent,
	    const string &	imageFileName,
	    bool 		animated = false );

    /**
     * Destructor.
     **/
    virtual ~YImage();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YImage"; }

    /**
     * Return the file name of this widget's image.
     **/
    string imageFileName() const;

    /**
     * Returns 'true' if the current image is an animated image format (e.g.,
     * MNG).
     **/
    bool animated() const;

    /**
     * Set and display a new image (or movie if animated is 'true').
     *
     * Derived classes should overwrite this, but call this base class function
     * in the new function.
     **/
    virtual void setImage( const string & imageFileName, bool animated = false );

    /**
     * Set and display a movie (an animated image).
     **/
    void setMovie( const string & movieFileName )
	{ setImage( movieFileName, true ); }

    /**
     * Return 'true' if the image widget should be stretchable with a default
     * width of 0 in the specified dimension. This is useful if the widget
     * width is determined by outside constraints, like the width of a
     * neighbouring widget.
     **/
    bool hasZeroSize( YUIDimension dim ) const;

    /**
     * Make the image widget stretchable with a default size of 0 in the
     * specified dimension. This is useful if the widget width is determined by
     * outside constraints, like the width of a neighbouring widget.
     *
     * This function is intentionally not virtual because it is only relevant
     * during the next geometry update, in which case the derived class has to
     * check this value anyway.
     **/
    void setZeroSize( YUIDimension dim, bool zeroSize = true );

    /**
     * Return 'true' if the image should be scaled to fit into the available
     * space.
     **/
    bool autoScale() const;

    /**
     * Make the image fit into the available space.
     *
     * Derived classes should overwrite this, but call this base class function
     * in the new function.
     **/
    virtual void setAutoScale( bool autoScale = true );


private:

    ImplPtr<YImagePrivate> priv;
};


#endif // YImage_h
