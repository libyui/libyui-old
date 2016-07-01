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
#include <YCheckBox.h>
#include <YRadioButton.h>
#include <YProgressBar.h>
#include <YRichText.h>
#include <YBusyIndicator.h>
#include <YSelectionBox.h>
#include <YMultiSelectionBox.h>
#include <YMultiLineEdit.h>
#include <YLabel.h>
#include <YLogView.h>
#include <YIntField.h>
#include <YImage.h>
#include <YSpacing.h>
#include <YFrame.h>
#include <YEmpty.h>
#include <YPackageSelector.h>
#include <YReplacePoint.h>
#include <YPropertyEditor.h>
#include <YPopupInternal.h>
#include <YAlignment.h>
#include <YCheckBoxFrame.h>
#include <YRadioButtonGroup.h>
#include <YUI.h>

#define TREE_VWEIGHT	40
#define PROP_VWEIGHT	60

#define DIA_HEIGHT	24

#define TREE_HEIGHT	10
#define TREE_WIDTH	50

#define PROP_HEIGHT	12
#define PROP_WIDTH	50

// helper methods
// static bool parentIsBox(YWidget *widget);
static bool isBox(YWidget *widget);


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




class YDialogSpyPrivate
{
public:

    YDialogSpyPrivate()
	: targetDialog( 0 )
	, spyDialog( 0 )
	, widgetTree( 0 )
	, propButton( 0 )
	, propReplacePoint( 0 )
	, propTable( 0 )
	{}

    ~YDialogSpyPrivate();

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


    YWidget * selectedWidget();
    void selectedWidgetChanged();
    void refreshProperties();
    bool toggleProperties();
    void highlightWidget(bool enable = true);

    void deleteWidget();
    void addWidget(const std::string &type);
    void editProperty();
    void moveSelectedUp() { moveSelected(true); }
    void moveSelectedDown() { moveSelected(false); }

private:
    void moveSelected(bool up);
    void showProperties();
    void hideProperties();
    bool propertiesShown() const;
    void targetDialogUpdated();
    void refreshButtonStates();
    void editWidget(YWidget *widget, const std::string &property="Label");
};

YDialogSpyPrivate::~YDialogSpyPrivate()
{
    highlightWidget(false);
}

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
    YMenuItem *m4 = new YMenuItem( "Alignment" );
    YMenuItem *m5 = new YMenuItem( "Size" );
    YMenuItem *m6 = new YMenuItem( "Containers" );
    YMenuItem *m7 = new YMenuItem( "Special" );
    add_items.push_back( m1 );
    add_items.push_back( m2 );
    add_items.push_back( m3 );
    add_items.push_back( m4 );
    add_items.push_back( m5 );
    add_items.push_back( m6 );
    add_items.push_back( m7 );

    new YMenuItem( m1, "BarGraph (GUI Only)" );
    new YMenuItem( m1, "Label" );
    new YMenuItem( m1, "Heading" );
    new YMenuItem( m1, "RichText" );
    new YMenuItem( m1, "ProgressBar" );
    new YMenuItem( m1, "BusyIndicator" );

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

    new YMenuItem( m4, "Left" );
    new YMenuItem( m4, "Right" );
    new YMenuItem( m4, "Top" );
    new YMenuItem( m4, "Bottom" );
    new YMenuItem( m4, "HCenter" );
    new YMenuItem( m4, "VCenter" );
    new YMenuItem( m4, "HVCenter" );

    new YMenuItem( m5, "MinHeight" );
    new YMenuItem( m5, "MinWidth" );
    new YMenuItem( m5, "MinSize" );
    new YMenuItem( m5, "HSquash" );
    new YMenuItem( m5, "VSquash" );
    new YMenuItem( m5, "HVSquash" );
    new YMenuItem( m5, "HWeight" );
    new YMenuItem( m5, "VWeight" );

    new YMenuItem( m6, "MarginBox" );
    new YMenuItem( m6, "ButtonBox" );
    new YMenuItem( m6, "CheckBoxFrame" );
    new YMenuItem( m6, "DumbTab" );
    new YMenuItem( m6, "Frame" );
    new YMenuItem( m6, "HBox" );
    new YMenuItem( m6, "HSpacing" );
    new YMenuItem( m6, "ReplacePoint" );
    new YMenuItem( m6, "VBox" );
    new YMenuItem( m6, "VSpacing" );

    new YMenuItem( m7, "TimezoneSelector" );
    new YMenuItem( m7, "PackageSelector" );

    priv->addButton->addItems( add_items );

    priv->deleteButton = fac->createPushButton( hbox, "&Delete" );
    priv->upButton = fac->createPushButton( hbox, "⬆ Up" );
    priv->downButton = fac->createPushButton( hbox, "⬇ Down" );

    priv->propReplacePoint = fac->createReplacePoint( vbox );
    fac->createEmpty( priv->propReplacePoint );

    priv->selectedWidgetChanged();
}

YDialogSpy::~YDialogSpy()
{
    if ( priv->spyDialog )
	priv->spyDialog->destroy();
}


bool YDialogSpyPrivate::propertiesShown() const
{
    return propTable != 0;
}

void YDialogSpyPrivate::highlightWidget(bool enable)
{
    if (targetDialog) targetDialog->highlight( enable ? selectedWidget() : 0);
}

void YDialogSpyPrivate::showProperties()
{
    if ( propertiesShown() ) return;

	propReplacePoint->deleteChildren();
	propReplacePoint->setWeight( YD_VERT, PROP_VWEIGHT );

	YWidgetFactory * fac = YUI::widgetFactory();
	YAlignment * minSize = fac->createMinSize( propReplacePoint,
						   PROP_WIDTH, PROP_HEIGHT );
	YTableHeader * header = new YTableHeader();
	YUI_CHECK_NEW( header );
	header->addColumn( "Property" );
	header->addColumn( "Value" );
	header->addColumn( "Type" );

	propTable = fac->createTable( minSize, header );
	propTable->setNotify( true );

	propButton->setLabel( "<<< &Properties" );
	propReplacePoint->showChild();
	spyDialog->recalcLayout();
}


void YDialogSpyPrivate::hideProperties()
{
    if ( !propertiesShown() ) return;

	propReplacePoint->deleteChildren();
	propReplacePoint->setWeight( YD_VERT, 0 );
	propTable = 0;
	YUI::widgetFactory()->createEmpty( propReplacePoint );

	propButton->setLabel( "&Properties >>>" );
	propReplacePoint->showChild();
	spyDialog->recalcLayout();
}

bool YDialogSpyPrivate::toggleProperties()
{
    bool ret = !propertiesShown();

    if (ret)
    {
        showProperties();
        refreshProperties();
    }
    else
        hideProperties();

    return ret;
}

void YDialogSpyPrivate::refreshProperties()
{
    // properties shown?
    if ( !propTable )
	return;

    propTable->deleteAllItems();

    auto widget = selectedWidget();
    if (!widget) return;

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
			propValStr = std::to_string(propVal.integerVal());
		    break;

		default:
		    propValStr = "???";
		    break;
	    }

	    YTableItem * item = new YTableItem( prop.name(), propValStr, prop.typeAsStr() );
	    YUI_CHECK_NEW( item );
	    items.push_back( item );
	}

	propTable->addItems( items );
	propTable->deselectAllItems();
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
        yuiMilestone() << "event: " << event;
        if (!event) continue;

        // window manager "close window" button
	    if ( event->eventType() == YEvent::CancelEvent ) break;
        else if ( event->eventType() == YEvent::MenuEvent)
        {
            YItem * menu_item = dynamic_cast<YItem *>(event->item());

            if (menu_item)
            {
                auto menu_label = menu_item->label();
                yuiMilestone() << "Activated menu item: " << menu_label << std::endl;
                priv->addWidget(menu_label);
            }

            continue;
        }
        else if ( event->widget() == priv->upButton ) priv->moveSelectedUp();
        else if ( event->widget() == priv->downButton)  priv->moveSelectedDown();
        else if ( event->widget() == priv->propButton ) priv->toggleProperties();
        else if ( event->widget() == priv->deleteButton) priv->deleteWidget();
        else if ( event->widget() == priv->propTable )  priv->editProperty();
	    else if ( event->widget() == priv->widgetTree ) priv->selectedWidgetChanged();
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

bool isVBox(YWidget *widget)
{
    auto box = dynamic_cast<YLayoutBox *>(widget);
    return box && box->primary() == YD_VERT;
}

/**
 * Is the widget placed in a VBox or Hbox?
 * (i.e. Is the parent widget a VBox or HBox?)
 * @param  widget the widget
 * @return        true if the widget is in a VBox or HBox
 */
// bool parentIsBox(YWidget *widget)
// {
//     auto parent = widget->parent();
//     return dynamic_cast<YLayoutBox *>(parent);
// }

YWidget * YDialogSpyPrivate::selectedWidget()
{
    YWidgetTreeItem * item = (YWidgetTreeItem *) widgetTree->selectedItem();

    return (item) ? item->widget() : 0;
}

void YDialogSpyPrivate::selectedWidgetChanged()
{
    highlightWidget();
    refreshProperties();
    refreshButtonStates();
}

void YDialogSpyPrivate::editProperty()
{
    YTableItem *selected_item = dynamic_cast<YTableItem *>(propTable->selectedItem());
    if (!selected_item) return;

    YTableCell *cell = selected_item->cell(0);
    yuiMilestone() << "editing property: " << cell->label();

    YPropertyEditor editor(selectedWidget());
    // update the property table when only the property has been changed
    if (editor.edit(cell->label())) refreshProperties();
}

void YDialogSpyPrivate::deleteWidget()
{
    YWidget *w = selectedWidget();
    if (!w) return;

    YWidget *parent = w->parent();
    if (!parent) return;

    yuiMilestone() << "removing widget: " << w << std::endl;
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

    targetDialogUpdated();
}

void YDialogSpyPrivate::moveSelected(bool up)
{
    auto target_widget = selectedWidget();
    if (!target_widget) return;

    auto parent = target_widget->parent();
    if (!parent || !isBox(parent)) return;

    if (up)
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
        // the last child cannot be moved further to the end
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

    targetDialogUpdated();
}

void YDialogSpyPrivate::addWidget(const std::string &type)
{
    auto widget = selectedWidget();
    if (!widget) return;

    try
    {
        auto f = YUI::widgetFactory();

        if (type == "Bottom")
            editWidget(f->createBottom(widget));
        else if (type == "BusyIndicator")
            editWidget(f->createBusyIndicator(widget, "Busy Indicator"));
        else if (type == "ButtonBox")
            editWidget(f->createButtonBox(widget));
        else if (type == "ComboBox")
            editWidget(f->createComboBox(widget, "Combo Box"));
        else if (type == "Empty")
            editWidget(f->createEmpty(widget));
        else if (type == "Frame")
            editWidget(f->createFrame(widget, "Frame"));
        else if (type == "HBox")
            editWidget(f->createHBox(widget));
        else if (type == "Heading")
            editWidget(f->createHeading(widget, "Heading"));
        else if (type == "HSpacing")
            editWidget(f->createHSpacing(widget));
        else if (type == "HStretch")
            editWidget(f->createHStretch(widget));
        else if (type == "CheckBox")
            editWidget(f->createCheckBox(widget, "Check Box"));
        else if (type == "CheckBoxFrame")
            // make it checked by default
            editWidget(f->createCheckBoxFrame(widget, "Check Box Frame", true));
        else if (type == "Image")
            editWidget(f->createImage(widget, ""));
        else if (type == "InputField")
            editWidget(f->createInputField(widget, "Input"));
        else if (type == "IntField")
            editWidget(f->createIntField(widget, "Integer Field", 0, 100, 50));
        else if (type == "Label")
            editWidget(f->createLabel(widget, "Label"));
        else if (type == "Left")
            editWidget(f->createLeft(widget));
        else if (type == "LogView")
            editWidget(f->createLogView(widget, "Log View", 12));
        else if (type == "MinHeight")
            editWidget(f->createMinHeight(widget, 10));
        else if (type == "MinWidth")
            editWidget(f->createMinWidth(widget, 10));
        else if (type == "MinSize")
            editWidget(f->createMinSize(widget, 10, 10));
        else if (type == "MultiLineEdit")
            editWidget(f->createMultiLineEdit(widget, "MultiLineEdit"));
        else if (type == "MultiSelectionBox")
            editWidget(f->createMultiSelectionBox(widget, "MultiSelection Box"));
        else if (type == "OutputField")
            editWidget(f->createOutputField(widget, "Output Field"));
        else if (type == "PackageSelector")
            editWidget(f->createPackageSelector(widget));
        else if (type == "Password")
            editWidget(f->createPasswordField(widget, "Password"));
        else if (type == "ProgressBar")
            editWidget(f->createProgressBar(widget, "Progress"));
        else if (type == "PushButton")
            editWidget(f->createPushButton(widget, "Button"));
        else if (type == "RadioButton")
            editWidget(f->createRadioButton(widget, "Radio Button"));
        else if (type == "RadioButtonGroup")
            editWidget(f->createRadioButtonGroup(widget));
        else if (type == "ReplacePoint")
            editWidget(f->createReplacePoint(widget));
        else if (type == "Right")
            editWidget(f->createRight(widget));
        else if (type == "RichText")
            editWidget(f->createRichText(widget, "This is a <b>RichText</b>."));
        else if (type == "SelectionBox")
            editWidget(f->createSelectionBox(widget, "Selection Box"));
        else if (type == "Top")
            editWidget(f->createTop(widget));
        else if (type == "Tree")
            editWidget(f->createTree(widget, "Tree"));
        else if (type == "VBox")
            editWidget(f->createVBox(widget));
        else if (type == "VSpacing")
            editWidget(f->createVSpacing(widget));
        else if (type == "VStretch")
            editWidget(f->createVStretch(widget));
        else
        {
            YPopupInternal::message(
                "Adding \"" + type + "\" widget type is not supported.");
            return;
        }

        targetDialogUpdated();
    }
    catch( const YUIException & exception )
    {
        YPopupInternal::message("Could not add a new widget:\n"
            + exception.msg());
    }
}

void YDialogSpyPrivate::targetDialogUpdated()
{
    // redraw the target dialog
    targetDialog->recalcLayout();

    // refresh the spy dialog
    widgetTree->deleteAllItems();
    fillWidgetTree(targetDialog, widgetTree);
}

void YDialogSpyPrivate::refreshButtonStates()
{
    auto widget = selectedWidget();
    auto parent = widget ? widget->parent() : 0;

    // Enable the moving buttons ony when the selected widget is inside
    // a VBox/HBox container, set the labels according to stacking direction.
    if (widget && parent && isBox(parent))
    {
        upButton->setEnabled(widget != parent->firstChild());
        upButton->setLabel(isVBox(parent) ? "⬆ Up" : "⬅ Left");
        downButton->setEnabled(widget != parent->lastChild());
        downButton->setLabel(isVBox(parent) ? "⬇ Down" : "➡ Right");
    }
    else
    {
        upButton->setEnabled(false);
        downButton->setEnabled(false);
    }

    // TODO: Enable the [Add] menu button only when a widget can be added
    // inside the current widget (i.e. it is a container).

    // Disable the [Delete] button when for the top level widget (YDialog)
    // TODO: disable it for the YQWizardButtons (Next, Back, ...), they cannot be
    // removed from the dialog.
    deleteButton->setEnabled(parent);
}

void YDialogSpyPrivate::editWidget(YWidget *widget, const std::string &property)
{
    // redraw the target dialog
    targetDialog->recalcLayout();

    if (!widget->propertySet().contains(property)) return;

    YPropertyEditor editor(widget);
    editor.edit(property);
}
