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
#include <YTable.h>
#include <YTree.h>
#include <YTreeItem.h>
#include <YLayoutBox.h>
#include <YAlignment.h>
#include <YButtonBox.h>
#include <YPushButton.h>
#include <YReplacePoint.h>
#include <YUI.h>

#define TREE_VWEIGHT	40
#define PROP_VWEIGHT	60

#define DIA_HEIGHT	24

#define TREE_HEIGHT	10
#define TREE_WIDTH	50

#define PROP_HEIGHT	12
#define PROP_WIDTH	50


/**
 * Custom tree item class to map tree items to widgets
 **/
class YWidgetTreeItem: public YTreeItem
{
public:
    YWidgetTreeItem( YWidget *	widget,
		     bool 	isOpen )
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
	, propButton( 0 )
	, propReplacePoint( 0 )
	, propTable( 0 )
	, closeButton( 0 )
	{}

    YDialog *		targetDialog;	// Dialog that is being inspected
    YDialog *		spyDialog;	// Debug dialog that shows widget data
    YTree *		widgetTree;	// Tree widget to show widget hierarchy
    YPushButton * 	propButton;
    YReplacePoint *	propReplacePoint;
    YTable *		propTable;
    YPushButton *	closeButton;
};



YDialogSpy::YDialogSpy( YDialog * targetDialog )
    : priv( new YDialogSpyPrivate() )
{
    if ( ! targetDialog )
	targetDialog = YDialog::topmostDialog();

    priv->targetDialog = targetDialog;
    YWidgetFactory * fac = YUI::widgetFactory();

    priv->spyDialog      = fac->createPopupDialog();
    YAlignment * diaMin	 = fac->createMinHeight( priv->spyDialog, DIA_HEIGHT );
    YLayoutBox * vbox    = fac->createVBox( diaMin );
    
    YAlignment * minSize = fac->createMinSize( vbox, TREE_WIDTH, TREE_HEIGHT );
    minSize->setWeight( YD_VERT, TREE_VWEIGHT );
    priv->widgetTree     = fac->createTree( minSize, "Widget &Tree" );
    priv->widgetTree->setNotify( true );

    YWidgetTreeItem * rootItem = new YWidgetTreeItem( targetDialog, true );
    YUI_CHECK_NEW( rootItem );
    fillTree( rootItem, targetDialog->childrenBegin(), targetDialog->childrenEnd(), 1 );
    priv->widgetTree->addItem( rootItem );
    priv->widgetTree->rebuildTree();

    YAlignment * alignment = fac->createLeft( vbox );
    priv->propButton       = fac->createPushButton( alignment, "&Properties >>>" );
    priv->propReplacePoint = fac->createReplacePoint( vbox );
    fac->createEmpty( priv->propReplacePoint );

    YButtonBox * buttonBox = fac->createButtonBox( vbox );
    priv->closeButton      = fac->createPushButton( buttonBox, "&Close" );
    priv->closeButton->setRole( YOKButton );
}


YDialogSpy::~YDialogSpy()
{
    if ( priv->spyDialog )
	priv->spyDialog->destroy();
}


bool YDialogSpy::propertiesShown() const
{
    return priv->propTable != 0;
}


void YDialogSpy::showProperties()
{
    if ( ! propertiesShown() )
    {
	priv->propReplacePoint->deleteChildren();
	priv->propReplacePoint->setWeight( YD_VERT, PROP_VWEIGHT );

	YWidgetFactory * fac = YUI::widgetFactory();
	YAlignment * minSize = fac->createMinSize( priv->propReplacePoint,
						   PROP_WIDTH, PROP_HEIGHT );
	YTableHeader * header = new YTableHeader();
	YUI_CHECK_NEW( header );
	header->addColumn( "Property" );
	header->addColumn( "Value" );
	header->addColumn( "Type" );
	
	priv->propTable = fac->createTable( minSize, header );
	// priv->propTable->setKeepSorting( true );
	
	priv->propButton->setLabel( "<<< &Properties" );
	priv->propReplacePoint->showChild();
	priv->spyDialog->recalcLayout();
    }
}


void YDialogSpy::hideProperties()
{
    if ( propertiesShown() )
    {
	priv->propReplacePoint->deleteChildren();
	priv->propReplacePoint->setWeight( YD_VERT, 0 );
	priv->propTable = 0;
	YUI::widgetFactory()->createEmpty( priv->propReplacePoint );
	
	priv->propButton->setLabel( "&Properties >>>" );
	priv->propReplacePoint->showChild();
	priv->spyDialog->recalcLayout();
    }
}


void YDialogSpy::showProperties( YWidget * widget )
{
    if ( ! priv->propTable )
	return;

    priv->propTable->deleteAllItems();

    if ( widget )
    {
	YPropertySet propSet = widget->propertySet();
	YItemCollection items;
	items.reserve( propSet.size() );
	
	for ( YPropertySet::const_iterator it = propSet.propertiesBegin();
	      it != propSet.propertiesEnd();
	      ++it )
	{
	    YProperty      prop    = *it;
	    YPropertyValue propVal = widget->getProperty( prop.name() );
	    string         propValStr;

	    switch ( prop.type() )
	    {
		case YStringProperty:
		    propValStr = propVal.stringVal();
		    break;

		case YBoolProperty:
		    propValStr = propVal.boolVal() ? "true" : "false";
		    break;

		case YIntegerProperty:
		    {
			std::ostringstream str;
			str << propVal.integerVal();
			propValStr = str.str();
		    }
		    break;

		default:
		    propValStr = "???";
		    break;
	    }
	    
	    YTableItem * item = new YTableItem( prop.name(), propValStr, prop.typeAsStr() );
	    YUI_CHECK_NEW( item );
	    items.push_back( item );
	}

	priv->propTable->addItems( items );
	priv->propTable->deselectAllItems();
    }
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
	bool updateProp = false;
	YEvent * event = priv->spyDialog->waitForEvent();
	yuiMilestone() << "dialog: " << priv->spyDialog->preferredHeight();
	yuiMilestone() << "tree: " << priv->widgetTree->preferredHeight();

	if ( event )
	{
	    if ( event->widget()    == priv->closeButton ||
		 event->eventType() == YEvent::CancelEvent ) // window manager "close window" button
	    {
		priv->targetDialog->highlight( 0 );
		return;
	    }

	    if ( event->widget() == priv->propButton )
	    {
		if ( propertiesShown() )
		    hideProperties();
		else
		{
		    showProperties();
		    updateProp = true;
		}
	    }
	    
	    if ( event->widget() == priv->widgetTree || updateProp )
	    {
		YWidgetTreeItem * item = (YWidgetTreeItem *) priv->widgetTree->selectedItem();
		yuiDebug() << "Highlighting " << item << endl;
		
		if ( item )
		{
		    priv->targetDialog->highlight( item->widget() );
		    showProperties( item->widget() );
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
