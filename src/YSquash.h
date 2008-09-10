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

  File:		YSquash.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YSquash_h
#define YSquash_h

#include "YSingleChildContainerWidget.h"
#include "ImplPtr.h"


class YSquashPrivate;

/**
 * HSquash, VSquash HVSquash:
 *
 * Squash is a widget that "squashes" its one child during layout, i.e., it
 * reduces it in size down to its preferred size. It may squash vertically,
 * horizontally or in both dimensions.
 **/
class YSquash : public YSingleChildContainerWidget
{
protected:
    /**
     * Constructor.
     *
     * Squashes horizontally if 'horSquash' is 'true',
     * vertically if 'vertSquash' is 'true'.
     **/
    YSquash( YWidget * parent, bool horSquash, bool vertSquash );

public:
    /**
     * Destructor.
     **/
    virtual ~YSquash();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const;

    /**
     * Returns 'true' if this widget squashes horizontally.
     **/
    bool horSquash() const;

    /**
     * Returns 'true' if this widget squashes vertically.
     **/
    bool vertSquash() const;


    /**
     * In a squashed dimension the widget NOT stretchable.
     * In an unsquashed dimension the widget is stretchable if the
     * child is stretchable.
     **/
    bool stretchable( YUIDimension dim ) const;

private:

    ImplPtr<YSquashPrivate> priv;
};


#endif // YSquash_h
