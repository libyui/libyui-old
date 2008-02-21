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

  File:		YWidgetFactory.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include "YWidgetFactory.h"
#include "YAlignment.h"
#include "YPushButton.h"
#include "YUI.h"
#include "YApplication.h"


YWidgetFactory::YWidgetFactory()
{
    // NOP
}

YWidgetFactory::~YWidgetFactory()
{
    // NOP
}


YDialog *
YWidgetFactory::createMainDialog( YDialogColorMode colorMode )
{
    return createDialog( YMainDialog, colorMode );
}


YDialog *
YWidgetFactory::createPopupDialog( YDialogColorMode colorMode )
{
    return createDialog( YPopupDialog, colorMode );
}


YLayoutBox *
YWidgetFactory::createVBox( YWidget * parent )
{
    return createLayoutBox( parent, YD_VERT );
}


YLayoutBox *
YWidgetFactory::createHBox( YWidget * parent )
{
    return createLayoutBox( parent, YD_HORIZ );
}


YSpacing *
YWidgetFactory::createHStretch( YWidget * parent )
{
    return createSpacing( parent,
			  YD_HORIZ,
			  true ); // stretchable
}


YSpacing *
YWidgetFactory::createVStretch( YWidget * parent )
{
    return createSpacing( parent,
			  YD_VERT,
			  true ); // stretchable
}


YSpacing *
YWidgetFactory::createHSpacing( YWidget * parent, YLayoutSize_t size )
{
    return createSpacing( parent,
			  YD_HORIZ,
			  false, // not stretchable
			  size );
}


YSpacing *
YWidgetFactory::createVSpacing( YWidget * parent, YLayoutSize_t size )
{
    return createSpacing( parent,
			  YD_VERT,
			  false, // not stretchable
			  size );
}


YAlignment *
YWidgetFactory::createLeft( YWidget * parent )
{
     return createAlignment( parent, YAlignBegin, YAlignUnchanged );
}


YAlignment *
YWidgetFactory::createRight( YWidget * parent )
{
     return createAlignment( parent, YAlignEnd,	YAlignUnchanged );
}


YAlignment *
YWidgetFactory::createTop( YWidget * parent )
{
     return createAlignment( parent, YAlignUnchanged, YAlignBegin );
}


YAlignment *
YWidgetFactory::createBottom( YWidget * parent )
{
     return createAlignment( parent, YAlignUnchanged, YAlignEnd	);
}


YAlignment *
YWidgetFactory::createHCenter( YWidget * parent )
{
     return createAlignment( parent, YAlignCenter, YAlignUnchanged );
}


YAlignment *
YWidgetFactory::createVCenter( YWidget * parent )
{
     return createAlignment( parent, YAlignUnchanged, YAlignCenter );
}


YAlignment *
YWidgetFactory::createHVCenter( YWidget * parent )
{
     return createAlignment( parent, YAlignCenter, YAlignCenter );
}


YAlignment *
YWidgetFactory::createMarginBox( YWidget * parent, YLayoutSize_t horMargin, YLayoutSize_t vertMargin )
{
     return createMarginBox( parent,
			     horMargin,  horMargin,
			     vertMargin, vertMargin );
}



YAlignment *
YWidgetFactory::createMarginBox( YWidget * parent,
				 YLayoutSize_t leftMargin, YLayoutSize_t rightMargin,
				 YLayoutSize_t topMargin,  YLayoutSize_t bottomMargin )
{
    YAlignment * alignment = createAlignment( parent, YAlignUnchanged, YAlignUnchanged );

    alignment->setLeftMargin  ( YUI::app()->deviceUnits( YD_HORIZ, leftMargin   ) );
    alignment->setRightMargin ( YUI::app()->deviceUnits( YD_HORIZ, rightMargin  ) );
    alignment->setTopMargin   ( YUI::app()->deviceUnits( YD_VERT,  topMargin    ) );
    alignment->setBottomMargin( YUI::app()->deviceUnits( YD_VERT,  bottomMargin ) );

    return alignment;
}


YAlignment *
YWidgetFactory::createMinWidth( YWidget * parent, YLayoutSize_t minWidth  )
{
    return createMinSize( parent, minWidth, 0  );
}


YAlignment *
YWidgetFactory::createMinHeight( YWidget * parent, YLayoutSize_t minHeight )
{
    return createMinSize( parent, 0, minHeight );
}


YAlignment *
YWidgetFactory::createMinSize( YWidget * parent, YLayoutSize_t minWidth, YLayoutSize_t minHeight )
{
    YAlignment * alignment = createAlignment( parent, YAlignUnchanged, YAlignUnchanged );

    alignment->setMinWidth ( YUI::app()->deviceUnits( YD_HORIZ, minWidth  ) );
    alignment->setMinHeight( YUI::app()->deviceUnits( YD_VERT,  minHeight ) );

    return alignment;
}


YSquash *
YWidgetFactory::createHSquash( YWidget * parent )
{
     return createSquash( parent, true,  false );
}


YSquash *
YWidgetFactory::createVSquash( YWidget * parent )
{
     return createSquash( parent, false, true );
}


YSquash *
YWidgetFactory::createHVSquash( YWidget * parent )
{
    return createSquash( parent, true,  true );
}


YPushButton *
YWidgetFactory::createIconButton( YWidget *	 parent,
				  const string & iconName,
				  const string & fallbackTextLabel )
{
    YPushButton * button = createPushButton( parent, fallbackTextLabel );
    button->setIcon( iconName );

    return button;
}


YLabel *
YWidgetFactory::createHeading( YWidget * parent, const string & text )
{
    return createLabel( parent,
			text,
			true,		// isHeading
			false );	// isOutputField
}


YLabel *
YWidgetFactory::createOutputField( YWidget * parent, const string & text )
{
    return createLabel( parent,
			text,
			false,		// isHeading
			true);		// isOutputField
}


YInputField *
YWidgetFactory::createPasswordField( YWidget * parent, const string & label )
{
    return createInputField( parent,
			     label,
			     true );	// passwordMode
}
