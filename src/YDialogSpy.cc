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

  File:		YDialogSpy.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <sstream>

#define YUILogComponent "ui-dialog-spy"
#include "YUILog.h"

#include <YDialogSpy.h>
#include <YWidgetFactory.h>
#include <YDialog.h>
#include <YEvent.h>
#include <YTree.h>
#include <YTreeItem.h>
#include <YLayoutBox.h>
#include <YAlignment.h>
#include <YButtonBox.h>
#include <YPushButton.h>
#include <YUI.h>


/**
 * Custom tree item class to map tree items to widgets
 **/
class YWidgetTreeItem: public YTreeItem
{
public:
    YWidgetTreeItem( YWidget * 		widget,
		     bool 		isOpen )
	: YTreeItem( "", isOpen )
	, _widget( widget )
    {
	setWidgetLabel();
    }

    YWidgetTreeItem( YWidgetTreeItem * 	parent,
		     YWidget * 		widget,
		     bool 		isOpen )
	: YTreeItem( parent, "", isOpen )
	, _widget( widget )
    {
	setWidgetLabel();
    }

    virtual ~YWidgetTreeItem() {}
    YWidget * widget() const { return _widget; }


protected:

    void setWidgetLabel()
    {
	std::ostringstream str;
	str << _widget;
	setLabel( str.str() );
    }

private:
    YWidget * _widget;
};


static void fillTree( YWidgetTreeItem * 		parent,
		      YWidgetListConstIterator 		begin,
		      YWidgetListConstIterator		end,
		      int				treeLevel );




struct YDialogSpyPrivate
{
    YDialogSpyPrivate()
	: targetDialog( 0 )
	, spyDialog( 0 )
	, widgetTree( 0 )
	, closeButton( 0 )
	{}

    YDialog *		targetDialog;	// Dialog that is being inspected
    YDialog *		spyDialog;	// Debug dialog that shows widget data
    YTree *		widgetTree;	// Tree widget to show widget hierarchy
    YPushButton *	closeButton;
};



YDialogSpy::YDialogSpy( YDialog * targetDialog )
    : priv( new YDialogSpyPrivate() )
{
    if ( ! targetDialog )
	targetDialog = YDialog::topmostDialog();

    priv->targetDialog = targetDialog;
    YWidgetFactory * fac = YUI::widgetFactory();

    priv->spyDialog = fac->createPopupDialog();
    YLayoutBox * vbox = fac->createVBox( priv->spyDialog );
    YAlignment * minSize = fac->createMinSize( vbox, 50, 20 );

    priv->widgetTree = fac->createTree( minSize, "Widget &Tree" );
    priv->widgetTree->setNotify( true );

    YWidgetTreeItem * rootItem = new YWidgetTreeItem( targetDialog, true );
    YUI_CHECK_NEW( rootItem );
    fillTree( rootItem, targetDialog->childrenBegin(), targetDialog->childrenEnd(), 1 );
    priv->widgetTree->addItem( rootItem );
    priv->widgetTree->rebuildTree();

    YButtonBox * buttonBox = fac->createButtonBox( vbox );
    priv->closeButton = fac->createPushButton( buttonBox, "&Close" );
    priv->closeButton->setRole( YOKButton );
}


YDialogSpy::~YDialogSpy()
{
    if ( priv->spyDialog )
	priv->spyDialog->destroy();
}


void fillTree( YWidgetTreeItem * 		parent,
	       YWidgetListConstIterator 	begin,
	       YWidgetListConstIterator		end,
	       int				treeLevel )
{
    for ( YWidgetListConstIterator it = begin; it != end; ++it )
    {
	YWidget * widget = *it;
	YWidgetTreeItem * item = new YWidgetTreeItem( parent, widget, treeLevel < 4 );

	if ( widget->hasChildren() )
	    fillTree( item, widget->childrenBegin(), widget->childrenEnd(), treeLevel+1 );
    }
}


void YDialogSpy::exec()
{
    YUI_CHECK_PTR( priv->spyDialog );

    while ( true )
    {
	YEvent * event = priv->spyDialog->waitForEvent();

	if ( event )
	{
	    if ( event->widget() == priv->closeButton )
	    {
		priv->targetDialog->highlight( 0 );
		return;
	    }

	    if ( event->widget() == priv->widgetTree )
	    {
		yuiDebug() << "Event from " << event->widget() << endl;
		YWidgetTreeItem * item = (YWidgetTreeItem *) priv->widgetTree->selectedItem();
		yuiDebug() << "Highlighting " << item << endl;
		
		if ( item )
		{
		    priv->targetDialog->highlight( item->widget() );
		}
	    }
	}
    }
}


void YDialogSpy::showDialogSpy( YDialog * dialog )
{
    try
    {
	YDialogSpy dialogSpy( dialog );
	dialogSpy.exec();
    }
    catch ( YUIException & exception )
    {
	YUI_CAUGHT( exception );
    }
}
