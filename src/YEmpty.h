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

  File:		YEmpty.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YEmpty_h
#define YEmpty_h

#include "YWidget.h"
#include "ImplPtr.h"


class YEmptyPrivate;


class YEmpty : public YWidget
{
protected:
    /**
     * Constructor.
     **/
    YEmpty( YWidget * parent );

public:
    /**
     * Destructor.
     **/
    virtual ~YEmpty();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YEmpty"; }

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

    ImplPtr<YEmptyPrivate> priv;
};


#endif // YEmpty_h
