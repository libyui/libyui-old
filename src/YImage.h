/*
  |****************************************************************************
  |
  | Copyright (c) 2000 - 2012 Novell, Inc.
  | All Rights Reserved.
  |
  | This program is free software; you can redistribute it and/or
  | modify it under the terms of version 2 of the GNU General Public License as
  | published by the Free Software Foundation.
  |
  | This program is distributed in the hope that it will be useful,
  | but WITHOUT ANY WARRANTY; without even the implied warranty of
  | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
  | GNU General Public License for more details.
  |
  | You should have received a copy of the GNU General Public License
  | along with this program; if not, contact Novell, Inc.
  |
  | To contact Novell about this file by physical or electronic mail,
  | you may find current contact information at www.novell.com
  |
  |****************************************************************************
*/



 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////   __/\\\\\\_____________/\\\__________/\\\________/\\\___/\\\________/\\\___/\\\\\\\\\\\_           ////
 ////    _\////\\\____________\/\\\_________\///\\\____/\\\/___\/\\\_______\/\\\__\/////\\\///__          ////
 ////     ____\/\\\______/\\\__\/\\\___________\///\\\/\\\/_____\/\\\_______\/\\\______\/\\\_____         ////
 ////      ____\/\\\_____\///___\/\\\_____________\///\\\/_______\/\\\_______\/\\\______\/\\\_____        ////
 ////       ____\/\\\______/\\\__\/\\\\\\\\\_________\/\\\________\/\\\_______\/\\\______\/\\\_____       ////
 ////        ____\/\\\_____\/\\\__\/\\\////\\\________\/\\\________\/\\\_______\/\\\______\/\\\_____      ////
 ////         ____\/\\\_____\/\\\__\/\\\__\/\\\________\/\\\________\//\\\______/\\\_______\/\\\_____     ////
 ////          __/\\\\\\\\\__\/\\\__\/\\\\\\\\\_________\/\\\_________\///\\\\\\\\\/_____/\\\\\\\\\\\_    ////
 ////           _\/////////___\///___\/////////__________\///____________\/////////______\///////////__   ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                 widget abstraction library providing Qt, GTK and ncurses frontends                  ////
 ////                                                                                                     ////
 ////                                   3 UIs for the price of one code                                   ////
 ////                                                                                                     ////
 ////                                       ***  core library  ***                                        ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                              (C) SUSE Linux GmbH    ////
 ////                                                                                                     ////
 ////                                                              libYUI-AsciiArt (C) 2012 Björn Esser   ////
 ////                                                                                                     ////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-/

  File:		YImage.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YImage_h
#define YImage_h

#include "YWidget.h"
#include <string>


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
	    const std::string &	imageFileName,
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
    std::string imageFileName() const;

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
    virtual void setImage( const std::string & imageFileName, bool animated = false );

    /**
     * Set and display a movie (an animated image).
     **/
    void setMovie( const std::string & movieFileName )
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
