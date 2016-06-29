/*
  Copyright (C) 2000-2012 Novell, Inc
  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) version 3.0 of the License. This library
  is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
  License for more details. You should have received a copy of the GNU
  Lesser General Public License along with this library; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
  Floor, Boston, MA 02110-1301 USA
*/


/*-/

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
#include <YMenuButton.h>
#include <YComboBox.h>
#include <YInputField.h>
#include <YLabel.h>
#include <YReplacePoint.h>
#include <YPropertyEditor.h>
#include <YPopupInternal.h>
#include <YUI.h>

#define TREE_VWEIGHT	40
#define PROP_VWEIGHT	60

#define DIA_HEIGHT	24

#define TREE_HEIGHT	10
#define TREE_WIDTH	50

#define PROP_HEIGHT	12
#define PROP_WIDTH	50

// helper methods
bool parentIsBox(YWidget *widget);
bool isBox(YWidget *widget);


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
	{}

    YDialog *		targetDialog;	// Dialog that is being inspected
    YDialog *		spyDialog;	// Debug dialog that shows widget data
    YTree *		widgetTree;	// Tree widget to show widget hierarchy
    YPushButton * 	propButton;
    YMenuButton * 	addButton;
    YPushButton * 	deleteButton;
    YPushButton * 	upButton;
    YPushButton * 	downButton;
    YReplacePoint *	propReplacePoint;
    YTable *		propTable;
};

void fillWidgetTree(YDialog *target, YTree *widgetTree)
{
    YWidgetTreeItem * rootItem = new YWidgetTreeItem( target, true );
    YUI_CHECK_NEW( rootItem );
    fillTree( rootItem, target->childrenBegin(), target->childrenEnd(), 1 );
    widgetTree->addItem( rootItem );
    widgetTree->rebuildTree();
}

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

    YAlignment * alignment = fac->createLeft( vbox );
    YMenuButton *fileMenu  = fac->createMenuButton( alignment, "&File" );

    YItemCollection items;
    YMenuItem *exp = new YMenuItem( "Export" );
    items.push_back( exp );
    fileMenu->addItems( items );

    YAlignment * minSize = fac->createMinSize( vbox, TREE_WIDTH, TREE_HEIGHT );
    minSize->setWeight( YD_VERT, TREE_VWEIGHT );
    priv->widgetTree     = fac->createTree( minSize, "Widget &Tree", false );
    priv->widgetTree->setNotify( true );

    fillWidgetTree(priv->targetDialog, priv->widgetTree);

    YLayoutBox * hbox = fac->createHBox( vbox );
    priv->propButton = fac->createPushButton( hbox, "&Properties >>>" );

    priv->addButton = fac->createMenuButton( hbox, "&Add" );
    YItemCollection add_items;
    YMenuItem *m1 = new YMenuItem( "Info" );
    YMenuItem *m2 = new YMenuItem( "Buttons" );
    YMenuItem *m3 = new YMenuItem( "Input" );
    YMenuItem *m4 = new YMenuItem( "Layout" );
    add_items.push_back( m1 );
    add_items.push_back( m2 );
    add_items.push_back( m3 );
    add_items.push_back( m4 );

    new YMenuItem( m1, "BarGraph (GUI Only)" );
    new YMenuItem( m1, "Label" );
    new YMenuItem( m1, "ProgressBar" );
    new YMenuItem( m1, "RichText" );

    new YMenuItem( m2, "PushButton" );
    new YMenuItem( m2, "CheckBox" );
    new YMenuItem( m2, "ComboBox" );
    new YMenuItem( m2, "MenuButton" );
    new YMenuItem( m2, "RadioButton" );

    new YMenuItem( m3, "DateField" );
    new YMenuItem( m3, "InputField" );
    new YMenuItem( m3, "IntField" );
    new YMenuItem( m3, "MultiLineEdit" );
    new YMenuItem( m3, "MultiSelectionBox" );
    new YMenuItem( m3, "Password" );
    new YMenuItem( m3, "SelectionBox" );
    new YMenuItem( m3, "Slider" );
    new YMenuItem( m3, "TimeField" );
    new YMenuItem( m3, "TimezoneSelector (GUI Only)" );

    new YMenuItem( m4, "ButtonBox" );
    new YMenuItem( m4, "CheckBoxFrame" );
    new YMenuItem( m4, "DumbTab" );
    new YMenuItem( m4, "Frame" );
    new YMenuItem( m4, "HBox" );
    new YMenuItem( m4, "HSpacing" );
    new YMenuItem( m4, "HSquash" );
    new YMenuItem( m4, "HWeight" );
    new YMenuItem( m4, "Left" );
    new YMenuItem( m4, "MarginBox" );
    new YMenuItem( m4, "MinHeight" );
    new YMenuItem( m4, "MinSize" );
    new YMenuItem( m4, "MinWidth" );
    new YMenuItem( m4, "ReplacePoint" );
    new YMenuItem( m4, "Right" );
    new YMenuItem( m4, "VBox" );
    new YMenuItem( m4, "VSpacing" );
    new YMenuItem( m4, "VSquash" );
    new YMenuItem( m4, "VWeight" );

    priv->addButton->addItems( add_items );

    priv->deleteButton = fac->createPushButton( hbox, "&Delete" );
    priv->upButton = fac->createPushButton( hbox, "⬆⬅ Begin" );
    priv->downButton = fac->createPushButton( hbox, "➡⬇ End" );

    priv->propReplacePoint = fac->createReplacePoint( vbox );
    fac->createEmpty( priv->propReplacePoint );
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
	priv->propTable->setNotify( true );

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
	    YProperty		prop    = *it;
	    YPropertyValue	propVal = widget->getProperty( prop.name() );
	    std::string		propValStr;

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
            yuiMilestone() << "event: " << event;

            // window manager "close window" button
    	    if ( event->eventType() == YEvent::CancelEvent )
    	    {
        		priv->targetDialog->highlight( 0 );
        		return;
    	    }

            YWidgetTreeItem * item = (YWidgetTreeItem *) priv->widgetTree->selectedItem();

            if ( event->eventType() == YEvent::MenuEvent)
            {
                auto fac = YUI::widgetFactory();

                if (event->item() && item)
                {
                    auto widget = item->widget();
                    YItem * menu_item = dynamic_cast<YItem *>(event->item());
                    auto menu_label = menu_item->label();
                    yuiMilestone() << "Activated menu: " << menu_label;
                    yuiMilestone().flush();

                    try
                    {
                        if (menu_label == "Label")
                        {
                            YLabel *label_widget = fac->createLabel(widget, "Label");
                            // redraw the target dialog
                            priv->targetDialog->recalcLayout();

                            YPropertyEditor editor(label_widget);
                            editor.edit("Label");

                            // refresh the spy dialog
                            priv->widgetTree->deleteAllItems();
                            fillWidgetTree(priv->targetDialog, priv->widgetTree);
                        }
                        else if (menu_label == "PushButton")
                        {
                            auto button_widget = fac->createPushButton(widget, "Button");
                            // redraw the target dialog
                            priv->targetDialog->recalcLayout();

                            YPropertyEditor editor(button_widget);
                            editor.edit("Label");

                            // refresh the spy dialog
                            priv->widgetTree->deleteAllItems();
                            fillWidgetTree(priv->targetDialog, priv->widgetTree);

                        }
                    }
                    catch( YUIException & exception )
                    {
                        YPopupInternal::message("Could not add a new widget:\n"
                            + exception.msg());
                    }
                }

                continue;
            }

            if ( event->widget() == priv->upButton || event->widget() == priv->downButton)
            {
                auto target_widget = item->widget();
                auto parent = target_widget->parent();

                if (isBox(parent))
                {
                    if (event->widget() == priv->upButton)
                    {
                        // the first child cannot be moved further
                        if (target_widget != parent->firstChild())
                        {
                            auto i = find( parent->childrenBegin(), parent->childrenEnd(), target_widget );
                            if (i != parent->childrenEnd())
                            {
                                // swap with the preceeding widget
                                std::swap(*(--i), *i);
                            }
                        }
                    }
                    else
                    {
                        // the last child cannot be moved further
                        if (target_widget != parent->lastChild())
                        {
                            auto i = find( parent->childrenBegin(), parent->childrenEnd(), target_widget );
                            if (i != parent->childrenEnd())
                            {
                                // swap with the succeeding widget
                                std::swap(*(++i), *i);
                            }
                        }
                    }

                    // redraw the target dialog
                    priv->targetDialog->recalcLayout();

                    // refresh the spy dialog
                    priv->widgetTree->deleteAllItems();
                    fillWidgetTree(priv->targetDialog, priv->widgetTree);
                }
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


            if ( event->widget() == priv->deleteButton && item)
            {
                YWidget *parent = item->widget()->parent();
                YWidget *w = item->widget();

                if (w && parent)
                {
                    yuiMilestone() << "removing widget: " << w;
                    parent->removeChild(w);

                    if ( w->isValid() )
                    {
                        delete w;
                    }

                    // any other child left after the removal?
                    if (!parent->hasChildren())
                    {
                        // add an Empty widget to have a valid widget tree
                        // e.g. empty VBoxes are not allowed
                        YUI::widgetFactory()->createEmpty(parent);
                    }

                    // redraw the target dialog
                    priv->targetDialog->recalcLayout();

                    // refresh the spy dialog
                    priv->widgetTree->deleteAllItems();
                    fillWidgetTree(priv->targetDialog, priv->widgetTree);
                }
            }

            if ( event->widget() == priv->propTable )
            {
                YTableItem *selected = dynamic_cast<YTableItem *>(priv->propTable->selectedItem());

                if (selected)
                {
                    YTableCell *cell = selected->cell(0);

                    YPropertyEditor editor(item->widget());
                    yuiMilestone() << "editing property: " << cell->label();
                    // update the property table when only the property has been changed
                    updateProp = editor.edit(cell->label());
                }
            }

    	    if ( event->widget() == priv->widgetTree || updateProp )
    	    {
        		yuiDebug() << "Highlighting " << item << std::endl;

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

/**
 * Is the widget a VBox or Hbox?
 * @param  widget the widget
 * @return        true if the widget is a VBox or HBox
 */
bool isBox(YWidget *widget)
{
    return dynamic_cast<YLayoutBox *>(widget);
}

/**
 * Is the widget placed in a VBox or Hbox?
 * (i.e. Is the parent widget a VBox or HBox?)
 * @param  widget the widget
 * @return        true if the widget is in a VBox or HBox
 */
bool parentIsBox(YWidget *widget)
{
    auto parent = widget->parent();
    return dynamic_cast<YLayoutBox *>(parent);
}
