/**************************************************************************
Copyright (C) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************/


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

  File:		YCPPropertyHandler.h

		Widget property handlers for not-so-trivial properties.

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <ycp/YCPBoolean.h>
#include <ycp/YCPInteger.h>
#include <ycp/YCPMap.h>
#include <ycp/YCPString.h>
#include <ycp/YCPTerm.h>
#include <ycp/YCPVoid.h>

#include <list>

#define y2log_component "ui"
#include <ycp/y2log.h>

#include "YCPPropertyHandler.h"

#include "YCPDialogParser.h"
#include "YCPItem.h"
#include "YCPTreeItem.h"
#include "YCPItemParser.h"
#include "YCPItemWriter.h"
#include "YCPMenuItemParser.h"
#include "YCPMenuItemWriter.h"
#include "YCPTableItemParser.h"
#include "YCPTableItemWriter.h"
#include "YCPTreeItemParser.h"
#include "YCPTreeItemWriter.h"
#include "YCPValueWidgetID.h"

#include <yui/YBarGraph.h>
#include <yui/YCheckBox.h>
#include <yui/YComboBox.h>
#include <yui/YDumbTab.h>
#include <yui/YMenuButton.h>
#include <yui/YMultiProgressMeter.h>
#include <yui/YMultiSelectionBox.h>
#include <yui/YRadioButton.h>
#include <yui/YRadioButtonGroup.h>
#include <yui/YSelectionBox.h>
#include <yui/YTable.h>
#include <yui/YTree.h>
#include <yui/YWizard.h>
#include <yui/YUISymbols.h>

using std::list;


bool
YCPPropertyHandler::setComplexProperty( YWidget *		widget,
					const string & 		propertyName,
					const YCPValue &	val )
{
    // y2debug( "%s::%s", widget->widgetClass(), propertyName.c_str() );

    if ( propertyName == YUIProperty_Value )
    {
	if ( trySetCheckBoxValue	( widget, val ) )		return true;
	if ( trySetSelectionBoxValue	( widget, val ) )		return true;
	if ( trySetTreeValue		( widget, val ) )		return true;
	if ( trySetTableValue		( widget, val ) )		return true;
	if ( trySetComboBoxValue	( widget, val ) )		return true;
	if ( trySetDumbTabValue		( widget, val ) )		return true;
	if ( trySetRadioButtonGroupCurrentButton( widget, val ) )	return true;
    }
    else if ( propertyName == YUIProperty_Values )
    {
	if ( trySetMultiProgressMeterValues ( widget, val ) )		return true;
	if ( trySetBarGraphValues       ( widget, val ) )		return true;
    }
    else if ( propertyName == YUIProperty_Labels )
    {
	if ( trySetBarGraphLabels	( widget, val ) )		return true;
    }
    else if ( propertyName == YUIProperty_CurrentItem )
    {
	if ( trySetSelectionBoxValue	( widget, val ) )		return true;
	if ( trySetTreeValue		( widget, val ) )		return true;
	if ( trySetTableValue		( widget, val ) )		return true;
	if ( trySetDumbTabValue		( widget, val ) )		return true;

	if ( trySetMultiSelectionBoxCurrentItem( widget, val ) )	return true;
    }
    else if ( propertyName == YUIProperty_Items )
    {
	// Make sure to try YMenuButton, YTable, YTree, before YSelectionWidget:
	// they all inherit YSelectionWidget!

	if ( trySetMenuButtonItems	( widget, val ) )		return true;
	if ( trySetTreeItems		( widget, val ) )		return true;
	if ( trySetTableItems		( widget, val ) )		return true;
	if ( trySetSelectionWidgetItems	( widget, val ) )		return true;
    }
    else if ( propertyName == YUIProperty_CurrentButton )
    {
	if ( trySetRadioButtonGroupCurrentButton( widget, val ) )	return true;
    }
    else if ( propertyName == YUIProperty_SelectedItems )
    {
	if ( trySetMultiSelectionBoxSelectedItems( widget, val ) )	return true;
	if ( trySetTableSelectedItems( widget, val ) )			return true;
	if ( trySetTreeSelectedItems( widget, val ) )			return true;
    }

    y2error( "Can't handle property %s::%s - not changing anything",
	     widget->widgetClass(), propertyName.c_str() );

    return false;
}


bool
YCPPropertyHandler::setComplexProperty( YWidget *		widget,
					const YCPTerm & 	propertyTerm,
					const YCPValue &	val 		)
{
    string propertyName = propertyTerm->name();

    if ( propertyName == YUIProperty_Cell ||
	 propertyName == YUIProperty_Item   )
    {
	if ( trySetTableCell( widget, propertyTerm, val ) )	return true;
    }

    y2error( "Can't handle property %s::%s - not changing anything",
	     widget->widgetClass(), propertyName.c_str() );

    return false;
}




YCPValue
YCPPropertyHandler::getComplexProperty( YWidget * widget, const string & propertyName )
{
    YCPValue val = YCPNull();

    if ( propertyName == YUIProperty_Value )
    {
	val = tryGetCheckBoxValue	( widget );	if ( ! val.isNull() ) return val;
	val = tryGetSelectionBoxValue	( widget );	if ( ! val.isNull() ) return val;
	val = tryGetTreeValue		( widget );	if ( ! val.isNull() ) return val;
	val = tryGetTableValue		( widget );	if ( ! val.isNull() ) return val;
	val = tryGetComboBoxValue	( widget );	if ( ! val.isNull() ) return val;
	val = tryGetDumbTabValue	( widget );	if ( ! val.isNull() ) return val;
        val = tryGetRadioButtonGroupCurrentButton( widget );	if ( ! val.isNull() ) return val;
    }
    else if ( propertyName == YUIProperty_Values )
    {
	val = tryGetBarGraphValues	( widget );	if ( ! val.isNull() ) return val;
    }
    else if ( propertyName == YUIProperty_CurrentItem )
    {
	val = tryGetSelectionBoxValue	( widget );	if ( ! val.isNull() ) return val;
	val = tryGetTreeCurrentItem	( widget );	if ( ! val.isNull() ) return val;
	val = tryGetTableValue		( widget );	if ( ! val.isNull() ) return val;
	val = tryGetComboBoxValue	( widget );	if ( ! val.isNull() ) return val;
	val = tryGetDumbTabValue	( widget );	if ( ! val.isNull() ) return val;
	val = tryGetWizardCurrentItem	( widget );	if ( ! val.isNull() ) return val;

	val = tryGetMultiSelectionBoxCurrentItem( widget ); if ( ! val.isNull() ) return val;
    }
    else if ( propertyName == YUIProperty_CurrentButton )
    {
	val = tryGetRadioButtonGroupCurrentButton( widget );	if ( ! val.isNull() ) return val;
    }
    else if ( propertyName == YUIProperty_SelectedItems )
    {
	val = tryGetMultiSelectionBoxSelectedItems( widget );	if ( ! val.isNull() ) return val;
	val = tryGetTableSelectedItems( widget );		if ( ! val.isNull() ) return val;
	val = tryGetTreeSelectedItems( widget );		if ( ! val.isNull() ) return val;
    }
    else if ( propertyName == YUIProperty_OpenItems )
    {
	val = tryGetTreeOpenItems	( widget );	if ( ! val.isNull() ) return val;
    }
    else if ( propertyName == YUIProperty_CurrentBranch )
    {
	val = tryGetTreeCurrentBranch	( widget );	if ( ! val.isNull() ) return val;
    }
    else if ( propertyName == YUIProperty_Items )
    {
	// Make sure to try YMenuButton, YTable, YTree, before YSelectionWidget:
	// they all inherit YSelectionWidget!

	val = tryGetMenuButtonItems	( widget );	if ( ! val.isNull() ) return val;
	val = tryGetTableItems		( widget );	if ( ! val.isNull() ) return val;
	val = tryGetTreeItems		( widget );	if ( ! val.isNull() ) return val;
	val = tryGetSelectionWidgetItems( widget );	if ( ! val.isNull() ) return val;
    }
    else if ( propertyName == YUIProperty_Labels )
    {
	val = tryGetBarGraphLabels	( widget );	if ( ! val.isNull() ) return val;
    }

    y2error( "Can't handle property %s::%s - returning 'nil'",
	     widget->widgetClass(), propertyName.c_str() );

    return YCPVoid();
}



YCPValue
YCPPropertyHandler::getComplexProperty( YWidget * widget, const YCPTerm & propertyTerm )
{
    YCPValue val = YCPNull();
    string propertyName = propertyTerm->name();

    if ( propertyName == YUIProperty_Cell ||
	 propertyName == YUIProperty_Item   )
    {
	val = tryGetTableCell	( widget, propertyTerm );	if ( ! val.isNull() ) return val;
    }

    y2error( "Can't handle property %s::%s - returning 'nil'",
	     widget->widgetClass(), propertyName.c_str() );

    return YCPVoid();
}






bool
YCPPropertyHandler::trySetCheckBoxValue( YWidget * widget, const YCPValue & val )
{
    YCheckBox * checkBox = dynamic_cast<YCheckBox *> (widget);

    if ( ! checkBox )
	return false;

    if ( val->isBoolean() )
    {
	checkBox->setChecked( val->asBoolean()->value() );
	return true;
    }
    else if ( val->isVoid() )
    {
	checkBox->setDontCare();
	return true;
    }

    YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_Value,
						      YOtherProperty ),
					   checkBox ) );
    return false;
}


template<class Item_t>
Item_t * findItem( const YCPValue &	wantedId,
		   YItemConstIterator	begin,
		   YItemConstIterator	end )
{
    for ( YItemConstIterator it = begin; it != end; ++it )
    {
	Item_t * item = dynamic_cast<Item_t *> (*it);

	if ( item && wantedId->equal( item->id() ) )
	    return item;

	if ( (*it)->hasChildren() )
	{
	    Item_t * result = findItem<Item_t>( wantedId,
						(*it)->childrenBegin(),
						(*it)->childrenEnd() );

	    if ( result )
		return result;
	}
    }

    return 0;
}


template<class Item_t>
Item_t * findItem( YSelectionWidget * selWidget, const YCPValue & val )
{
    YCPValue wantedId( val );

    if ( val->isTerm() && val->asTerm()->name() == YUISymbol_id )	// `id(..) ?
    {
	wantedId = val->asTerm()->value(0);
    }

    return findItem<Item_t>( wantedId, selWidget->itemsBegin(), selWidget->itemsEnd() );
}


template<class Widget_t, class Item_t>
bool trySetSelectionWidgetValue( YWidget * widget, const YCPValue & val )
{
    Widget_t * selWidget = dynamic_cast<Widget_t *> (widget);

    if ( ! selWidget )
	return false;

    if ( val->isVoid() )	// Special case: nil deselects everything
    {
	selWidget->deselectAllItems();
	return true;
    }


    // Find the item with the specified ID

    Item_t * item = findItem<Item_t>( selWidget, val );

    if ( item )
    {
	selWidget->selectItem( item, true );
    }
    else
    {
	y2error( "%s \"%s\" has no item with ID %s",
		 widget->widgetClass(),
		 widget->debugLabel().c_str(),
		 val->toString().c_str() );
    }

    return true;
}


bool
YCPPropertyHandler::trySetSelectionBoxValue( YWidget * widget, const YCPValue & val )
{
    return trySetSelectionWidgetValue<YSelectionBox, YCPItem>( widget, val );
}


bool
YCPPropertyHandler::trySetTreeValue( YWidget * widget, const YCPValue & val )
{
    return trySetSelectionWidgetValue<YTree, YCPTreeItem>( widget, val );
}


bool
YCPPropertyHandler::trySetTableValue( YWidget * widget, const YCPValue & val )
{
    return trySetSelectionWidgetValue<YTable, YCPTableItem>( widget, val );
}


bool
YCPPropertyHandler::trySetDumbTabValue( YWidget * widget, const YCPValue & val )
{
    return trySetSelectionWidgetValue<YDumbTab, YCPItem>( widget, val );
}


bool
YCPPropertyHandler::trySetComboBoxValue( YWidget * widget, const YCPValue & val )
{
    YComboBox * comboBox = dynamic_cast<YComboBox *> (widget);

    if ( ! comboBox )
	return false;

    YCPItem * item = findItem<YCPItem>( comboBox, val );

    if ( item )
    {
	comboBox->selectItem( item, true );
    }
    else
    {
	if ( comboBox->editable() )
	{
	    if ( val->isString() )
	    {
		comboBox->setValue( val->asString()->value() );
		return true;
	    }
	    else if ( val->isInteger() )
	    {
		comboBox->setValue( val->toString() );
		return true;
	    }
	}

	y2error( "%s %s has no item with ID %s",
		 widget->widgetClass(),
		 widget->debugLabel().c_str(),
		 val->toString().c_str() );
    }

    return true;
}


bool
YCPPropertyHandler::trySetSelectionWidgetItems( YWidget * widget, const YCPValue & val )
{
    YSelectionWidget * selWidget = dynamic_cast<YSelectionWidget *> (widget );

    if ( ! selWidget )
	return false;

    if ( val->isList() )
    {
	selWidget->setItems( YCPItemParser::parseItemList( val->asList() ) );
	return true;
    }

    YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_Items,
						      YOtherProperty ),
					   widget ) );
    return false;
}


bool
YCPPropertyHandler::trySetMenuButtonItems( YWidget * widget, const YCPValue & val )
{
    YMenuButton * menuButton = dynamic_cast<YMenuButton *> (widget );

    if ( ! menuButton )
	return false;

    if ( val->isList() )
    {
	menuButton->setItems( YCPMenuItemParser::parseMenuItemList( val->asList() ) );
	return true;
    }

    YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_Items,
						      YOtherProperty ),
					   widget ) );
    return false;
}


bool
YCPPropertyHandler::trySetTreeItems( YWidget * widget, const YCPValue & val )
{
    YTree * tree = dynamic_cast<YTree *> (widget );

    if ( ! tree )
	return false;

    if ( val->isList() )
    {
	tree->setItems( YCPTreeItemParser::parseTreeItemList( val->asList() ) );
	return true;
    }

    YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_Items,
						      YOtherProperty ),
					   widget ) );
    return false;
}


bool
YCPPropertyHandler::trySetTableItems( YWidget * widget, const YCPValue & val )
{
    YTable * table = dynamic_cast<YTable *> (widget );

    if ( ! table )
	return false;

    if ( val->isList() )
    {
	table->setItems( YCPTableItemParser::parseTableItemList( val->asList() ) );
	return true;
    }

    YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_Items,
						      YOtherProperty ),
					   widget ) );
    return false;
}


bool
YCPPropertyHandler::trySetTableCell( YWidget * widget, const YCPTerm & propTerm, const YCPValue & val )
{
    YTable * table = dynamic_cast<YTable *> (widget );

    if ( ! table )
	return false;

    if ( propTerm->size() == 2 &&
	 propTerm->value(1)->isInteger() )
    {
	YCPValue	itemId = propTerm->value(0);
	int		column = propTerm->value(1)->asInteger()->value();

	YCPTableItem * item = findItem<YCPTableItem>( table, itemId );

	if ( item )
	{
	    YTableCell * cell = item->cell( column );

	    if ( cell )
	    {
		if ( val->isString() )
		{
		    cell->setLabel( val->asString()->value() );
		    table->cellChanged( cell );
		}
		else if ( val->isInteger() )	// for backwards compatibility
		{
		    cell->setLabel( val->toString().c_str() );
		    table->cellChanged( cell );
		}
		else if ( val->isVoid() )	// for backwards compatibility
		{
		    cell->setLabel( "" );
		    table->cellChanged( cell );
		}
		else if ( val->isTerm()
			  && val->asTerm()->name() == YUISymbol_icon // `icon()
			  && val->asTerm()->size() == 1
			  && val->asTerm()->value(0)->isString() )
		{
		    YCPString iconName = val->asTerm()->value(0)->asString();
		    cell->setIconName( iconName->value().c_str() );
		    table->cellChanged( cell );
		}
		else // any other type
		{
		    string msg = "Expected string, integer, or `icon(\"myicon.png\"), not ";
		    msg += val->toString().c_str();
		    YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_Item,
								      YOtherProperty ),
							   table, msg ) );
		}
	    }
	    else // ! cell
	    {
		y2error( "%s item %s doesn't have a column #%d",
			 table->widgetClass(),
			 itemId->toString().c_str(),
			 column );
	    }
	}
	else // ! item
	{
	    y2error( "%s %s has no item with ID %s",
		     table->widgetClass(),
		     table->debugLabel().c_str(),
		     itemId->toString().c_str() );
	}

	return true;
    }

    YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_Items,
						      YOtherProperty ),
					   widget ) );
    return false;
}


bool
YCPPropertyHandler::trySetRadioButtonGroupCurrentButton( YWidget * widget, const YCPValue & val )
{
    YRadioButtonGroup * radioButtonGroup = dynamic_cast<YRadioButtonGroup *> (widget);

    if ( ! radioButtonGroup )
	return false;

    YWidget * refWidget = YCPDialogParser::findWidgetWithId( radioButtonGroup, val );
    // This will throw an exception if no widget with that ID found

    YRadioButton * newCurrentButton = dynamic_cast<YRadioButton *> (refWidget);

    if ( newCurrentButton )
    {
	// y2debug( "Setting new current radio button: %s", newCurrentButton->debugLabel().c_str() );
	newCurrentButton->setValue( true );
    }
    else
    {
	string msg = string( "Expected a YRadioButton, not a " ) + widget->widgetClass();
	YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_CurrentButton,
							  YOtherProperty ),
					       widget, msg ) );
    }

    return true;
}


template<class Widget_t, class Item_t>
bool trySetSelectionWidgetSelectedItems( YWidget * widget, const YCPValue & val )
{
    Widget_t * selWidget = dynamic_cast<Widget_t *> (widget);

    if ( ! selWidget )
	return false;

    if ( ! val->isList() )
    {
	string msg = "Expected a list of item IDs";
	YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_SelectedItems,
							  YOtherProperty ),
					       widget, msg ) );

    }

    YCPList itemIdList = val->asList();

    selWidget->deselectAllItems();

    for ( int i=0; i < itemIdList.size(); i++ )
    {
	Item_t * item = findItem<Item_t>( selWidget, itemIdList->value( i ) );

	if ( ! item )
	{
	    y2error( "%s \"%s\" has no item with ID %s",
		     selWidget->widgetClass(),
		     selWidget->debugLabel().c_str(),
		     itemIdList->value(i)->toString().c_str() );
	}
	else
	{
	    selWidget->selectItem( item );
	}
    }

    return true;
}


bool
YCPPropertyHandler::trySetMultiSelectionBoxSelectedItems( YWidget * widget, const YCPValue & val )
{
    return trySetSelectionWidgetSelectedItems<YMultiSelectionBox, YCPItem>( widget, val );
}


bool
YCPPropertyHandler::trySetTableSelectedItems( YWidget * widget, const YCPValue & val )
{
    return trySetSelectionWidgetSelectedItems<YTable, YCPTableItem>( widget, val );
}


bool
YCPPropertyHandler::trySetTreeSelectedItems( YWidget * widget, const YCPValue & val )
{
    return trySetSelectionWidgetSelectedItems<YTree, YCPTreeItem>( widget, val );
}


bool
YCPPropertyHandler::trySetMultiSelectionBoxCurrentItem( YWidget * widget, const YCPValue & val )
{
    YMultiSelectionBox * multiSelBox = dynamic_cast<YMultiSelectionBox *> (widget);

    if ( ! multiSelBox )
	return false;

    if ( val->isVoid() )	// Special case: nil sets "no selected item"
    {
	multiSelBox->setCurrentItem( 0 );
	return true;
    }

    // Find the item with the specified ID

    YCPItem * item = findItem<YCPItem>( multiSelBox , val );

    if ( item )
    {
	multiSelBox->setCurrentItem( item );
    }
    else
    {
	y2error( "%s \"%s\" has no item with ID %s",
		 widget->widgetClass(),
		 widget->debugLabel().c_str(),
		 val->toString().c_str() );
    }

    return true;
}


bool
YCPPropertyHandler::trySetMultiProgressMeterValues( YWidget * widget, const YCPValue & val )
{
    YMultiProgressMeter * multiProgressMeter = dynamic_cast<YMultiProgressMeter *> (widget);

    if ( ! multiProgressMeter )
	return false;

    if ( ! val->isList() )
    {
	string msg = "Expected a list of integers or floats";
	YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_Values,
							  YOtherProperty ),
					       widget, msg ) );

    }

    multiProgressMeter->setCurrentValues( YCPDialogParser::parseNumList( val->asList() ) );

    return true;
}


bool
YCPPropertyHandler::trySetBarGraphValues( YWidget * widget, const YCPValue & val )
{
    YBarGraph * barGraph = dynamic_cast<YBarGraph *> (widget);

    if ( ! barGraph )
	return false;

    string msg = "Expected a list of integers";

    if ( ! val->isList() )
    {
	YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_Values,
							  YOtherProperty ),
					       widget, msg ) );
    }

    YBarGraphMultiUpdate multiUpdate( barGraph );

    YCPList valuesList = val->asList();

    for ( int i=0; i < valuesList->size(); i++ )
    {
	if ( valuesList->value(i)->isInteger() )
	{
	    if ( i < barGraph->segments() )
		barGraph->setValue( i, valuesList->value(i)->asInteger()->value() );
	    else
	    {
		ycperror( "Ignoring excess BarGraph value #%i (only %d segments!)",
			  i, barGraph->segments() );
	    }
	}
	else
	{
	    YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_Values,
							      YOtherProperty ),
						   widget, msg ) );
	}
    }

    return true;
}


bool
YCPPropertyHandler::trySetBarGraphLabels( YWidget * widget, const YCPValue & val )
{
    YBarGraph * barGraph = dynamic_cast<YBarGraph *> (widget);

    if ( ! barGraph )
	return false;

    string msg = "Expected a list of strings";

    if ( ! val->isList() )
    {
	YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_Values,
							  YOtherProperty ),
					       widget, msg ) );
    }

    YBarGraphMultiUpdate multiUpdate( barGraph );

    YCPList labelsList = val->asList();

    for ( int i=0; i < labelsList->size(); i++ )
    {
	if ( labelsList->value(i)->isString() )
	{
	    if ( i < barGraph->segments() )
		barGraph->setLabel( i, labelsList->value(i)->asString()->value() );
	    else
	    {
		ycperror( "Ignoring excess BarGraph label #%i (only %d segments!)",
			  i, barGraph->segments() );
	    }
	}
	else
	{
	    YUI_THROW( YUIBadPropertyArgException( YProperty( YUIProperty_Values,
							      YOtherProperty ),
						   widget, msg ) );
	}
    }

    return true;
}





//------------------------------------------------------------------------------



YCPValue
YCPPropertyHandler::tryGetCheckBoxValue( YWidget * widget )
{
    YCheckBox * checkBox = dynamic_cast<YCheckBox *> (widget);

    if ( ! checkBox )
	return YCPNull();

    switch ( checkBox->value() )
    {
	case YCheckBox_on:		return YCPBoolean( true  );
	case YCheckBox_off:		return YCPBoolean( false );
	case YCheckBox_dont_care:	return YCPVoid();	// nil
    }

    return YCPVoid();	// shouldn't happen
}


template<class Widget_t, class Item_t >
YCPValue tryGetSelectionWidgetValue( YWidget * widget )
{
    Widget_t * selWidget = dynamic_cast<Widget_t *> (widget);

    if ( ! selWidget )
	return YCPNull();

    YItem * selectedItem = selWidget->selectedItem();

    if ( selectedItem )
    {
	Item_t * item = dynamic_cast<Item_t *> (selectedItem);

	if ( item )
	    return item->id();
	else
	    y2error( "Item is not a YCPItem: \"%s\"", selectedItem->label().c_str() );
    }

    return YCPVoid();
}


YCPValue
YCPPropertyHandler::tryGetSelectionBoxValue( YWidget * widget )
{
    return tryGetSelectionWidgetValue<YSelectionBox, YCPItem>( widget );
}


YCPValue
YCPPropertyHandler::tryGetTreeValue( YWidget * widget )
{
    return tryGetSelectionWidgetValue<YTree, YCPTreeItem>( widget );
}


YCPValue
YCPPropertyHandler::tryGetTableValue( YWidget * widget )
{
    return tryGetSelectionWidgetValue<YTable, YCPTableItem>( widget );
}


YCPValue
YCPPropertyHandler::tryGetDumbTabValue( YWidget * widget )
{
    return tryGetSelectionWidgetValue<YDumbTab, YCPItem>( widget );
}


YCPValue
YCPPropertyHandler::tryGetComboBoxValue( YWidget * widget )
{
    YCPValue result = tryGetSelectionWidgetValue<YComboBox, YCPItem>( widget );

    if ( ! result.isNull() && result->isVoid() )
    {
	YComboBox * comboBox = dynamic_cast<YComboBox *> (widget);

	if ( ! comboBox )
	    return YCPNull();

	result = YCPString( comboBox->value() );
    }

    return result;
}


YCPValue
YCPPropertyHandler::tryGetRadioButtonGroupCurrentButton( YWidget * widget )
{
    YRadioButtonGroup * radioButtonGroup = dynamic_cast<YRadioButtonGroup *> (widget);

    if ( ! radioButtonGroup )
	return YCPNull();

    YRadioButton * currentButton = radioButtonGroup->currentButton();

    if ( ! currentButton )
	return YCPVoid();

    // y2debug( "Current radio button: %s", currentButton->debugLabel().c_str() );

    if ( currentButton->hasId() )
    {
	YCPValueWidgetID * id = dynamic_cast<YCPValueWidgetID *> ( currentButton->id() );

	if ( id )
	    return id->value();
    }

    y2warning( "%s \"%s\" has no ID",
	       currentButton->widgetClass(),
	       currentButton->debugLabel().c_str() );

    return YCPVoid();
}


template<class Widget_t, class Item_t >
YCPValue tryGetSelectionWidgetSelectedItems( YWidget * widget )
{
    Widget_t * selWidget = dynamic_cast<Widget_t *> (widget);

    if ( ! selWidget )
	return YCPNull();

    YCPList selectedItemsList;
    YItemCollection selectedItems = selWidget->selectedItems();

    for ( YItemIterator it = selectedItems.begin();
	  it != selectedItems.end();
	  ++it )
    {
	const Item_t * item = dynamic_cast<const Item_t *> (*it);

	if ( item )
	{
	    if ( item->hasId() )
		selectedItemsList->add( item->id() );
	    else
		y2error( "Item has no ID: %s", (*it)->label().c_str() );
	}
	else
	    y2error( "Wrong item type (not a YCPItem): %s", (*it)->label().c_str() );
    }

    return selectedItemsList;
}


YCPValue
YCPPropertyHandler::tryGetMultiSelectionBoxSelectedItems( YWidget * widget )
{
    return tryGetSelectionWidgetSelectedItems<YMultiSelectionBox, YCPItem>( widget );
}


YCPValue
YCPPropertyHandler::tryGetTableSelectedItems( YWidget * widget )
{
    return tryGetSelectionWidgetSelectedItems<YTable, YCPTableItem>( widget );
}

YCPValue
YCPPropertyHandler::tryGetTreeSelectedItems( YWidget * widget )
{
    return tryGetSelectionWidgetSelectedItems<YTree, YCPTreeItem>( widget );
}



YCPValue
YCPPropertyHandler::tryGetMultiSelectionBoxCurrentItem( YWidget * widget )
{
    YMultiSelectionBox * multiSelBox = dynamic_cast<YMultiSelectionBox *> (widget);

    if ( ! multiSelBox )
	return YCPNull();

    YItem * currentItem = multiSelBox->currentItem();

    if ( currentItem )
    {
	YCPItem * item = dynamic_cast<YCPItem *> (currentItem);

	if ( item )
	    return item->id();
	else
	    y2error( "Item is not a YCPItem: \"%s\"", currentItem->label().c_str() );
    }

    return YCPVoid();
}


YCPValue
YCPPropertyHandler::tryGetWizardCurrentItem( YWidget * widget )
{
    YWizard * wizard = dynamic_cast<YWizard *> (widget);

    if ( ! wizard )
	return YCPNull();

    return YCPString( wizard->currentTreeSelection() );
}


YCPValue
YCPPropertyHandler::tryGetTreeOpenItems( YWidget * widget )
{
    YTree * tree = dynamic_cast<YTree *> (widget);

    if ( ! tree )
	return YCPNull();

    YCPMap openItems;
    getTreeOpenItems( openItems, tree->itemsBegin(), tree->itemsEnd() );

    return openItems;
}


void
YCPPropertyHandler::getTreeOpenItems( YCPMap &			openItems,
				      YItemConstIterator 	begin,
				      YItemConstIterator 	end )
{
    for ( YItemConstIterator it = begin; it != end; ++it )
    {
	YTreeItem * item = dynamic_cast<YTreeItem *> (*it);

	if ( item )
	{
	    YCPTreeItem * ycpTreeItem = dynamic_cast<YCPTreeItem *> (item);

	    if ( item->isOpen() )
	    {
		if ( ycpTreeItem && ycpTreeItem->hasId() )
		    openItems.add( ycpTreeItem->id(), YCPString( "ID" ) );
		else
		    openItems.add( YCPString( item->label() ), YCPString( "Text" ) );
	    }

	    if ( item->hasChildren() )
		getTreeOpenItems( openItems, item->childrenBegin(), item->childrenEnd() );
	}
    }
}


YCPValue
YCPPropertyHandler::tryGetTreeCurrentBranch( YWidget * widget )
{
    YTree * tree = dynamic_cast<YTree *> (widget);

    if ( ! tree )
	return YCPNull();

    // YTree::CurrentBranch: The path from the root to the current item as a
    // list of IDs or, if an item doesn't have an ID, its label
    //
    //     "/" -> `usr -> `share -> "doc"
    //
    // -> [ "/", `usr, `share, "doc" ]

    list<YTreeItem *> path;
    YItem * item = tree->selectedItem();

    if ( item )
    {
	YTreeItem * treeItem = dynamic_cast<YTreeItem *> (item);

	while ( treeItem )
	{
	    path.push_front( treeItem ); // prepend list
	    treeItem = treeItem->parent();
	}
    }

    YCPList currentBranch;

    for ( list<YTreeItem *>::iterator it = path.begin(); it != path.end(); ++it )
    {
	YCPTreeItem * ycpTreeItem = dynamic_cast<YCPTreeItem *> (*it);

	if ( ycpTreeItem && ycpTreeItem->hasId() )
	    currentBranch.add( ycpTreeItem->id() );
	else
	    currentBranch.add( YCPString( (*it)->label() ) );
    }

    return currentBranch;
}


YCPValue
YCPPropertyHandler::tryGetTableCell( YWidget * widget, const YCPTerm & propTerm )
{
    YTable * table = dynamic_cast<YTable *> (widget);

    if ( ! table )
	return YCPNull();

    YCPValue result	= YCPVoid();
    string   property	= propTerm->name();

    if ( property == YUIProperty_Item && propTerm->size() == 1 ) // `Item("myItemID")
    {
	return tryGetTableItem( widget, propTerm );
    }

    if ( propTerm->size() != 2 ||				// `Item("myItemID", index)
	 ! propTerm->value(1)->isInteger() )
    {
	YUI_THROW( YUIBadPropertyArgException( YProperty( property, YOtherProperty ),
					       table ) );
    }

    YCPValue	itemId = propTerm->value(0);
    int		column = propTerm->value(1)->asInteger()->value();

    const YCPTableItem * item = findItem<YCPTableItem>( table, itemId );

    if ( item )
    {
	const YTableCell * cell = item->cell( column );

	if ( cell )
	{
	    result = YCPTableItemWriter::cellValue( cell );
	}
	else // ! cell
	{
	    y2error( "%s item %s doesn't have a column #%d",
		     table->widgetClass(),
		     itemId->toString().c_str(),
		     column );
	}
    }
    else // ! item
    {
	y2error( "%s %s has no item with ID %s",
		 table->widgetClass(),
		 table->debugLabel().c_str(),
		 itemId->toString().c_str() );
    }

    return result;
}


YCPValue
YCPPropertyHandler::tryGetTableItem( YWidget * widget, const YCPTerm & propTerm )
{
    YTable * table = dynamic_cast<YTable *> (widget);

    if ( ! table )
	return YCPNull();

    YCPValue	result	 = YCPVoid();
    string	property = propTerm->name();

    if ( propTerm->size() != 1 )
    {
	YUI_THROW( YUIBadPropertyArgException( YProperty( property, YOtherProperty ),
					       table ) );
    }

    YCPValue itemId = propTerm->value(0);
    const YCPTableItem * item = findItem<YCPTableItem>( table, itemId );

    if ( item )
    {
	result = YCPTableItemWriter::itemTerm( item );
    }
    else // ! item
    {
	y2error( "%s %s has no item with ID %s",
		 table->widgetClass(),
		 table->debugLabel().c_str(),
		 itemId->toString().c_str() );
    }

    return result;
}


YCPValue
YCPPropertyHandler::tryGetMenuButtonItems( YWidget * widget )
{
    YMenuButton * menuButton = dynamic_cast<YMenuButton *> (widget);

    if ( ! menuButton )
	return YCPNull();

    return YCPMenuItemWriter::itemList( menuButton->itemsBegin(), menuButton->itemsEnd() );
}


YCPValue
YCPPropertyHandler::tryGetTableItems( YWidget * widget )
{
    YTable * table = dynamic_cast<YTable *> (widget);

    if ( ! table )
	return YCPNull();

    return YCPTableItemWriter::itemList( table->itemsBegin(), table->itemsEnd() );
}

YCPValue
YCPPropertyHandler::tryGetTreeCurrentItem( YWidget * widget )
{
    YTree * tree = dynamic_cast<YTree *> (widget);

    if ( ! tree )
        return YCPNull();

    YItem * currentItem = tree->currentItem();
    YCPTreeItem * item = dynamic_cast<YCPTreeItem *> (currentItem);

    if ( item )
        return item->id();

    return YCPVoid();

}



YCPValue
YCPPropertyHandler::tryGetTreeItems( YWidget * widget )
{
    YTree * tree = dynamic_cast<YTree *> (widget);

    if ( ! tree )
	return YCPNull();

    return YCPTreeItemWriter::itemList( tree->itemsBegin(), tree->itemsEnd() );
}


YCPValue
YCPPropertyHandler::tryGetSelectionWidgetItems( YWidget * widget )
{
    YSelectionWidget * selWidget = dynamic_cast<YSelectionWidget *> (widget);

    if ( ! selWidget )
	return YCPNull();

    return YCPItemWriter::itemList( selWidget->itemsBegin(), selWidget->itemsEnd() );
}


YCPValue
YCPPropertyHandler::tryGetBarGraphValues( YWidget * widget )
{
    YBarGraph * barGraph = dynamic_cast<YBarGraph *> (widget);

    if ( ! barGraph )
	return YCPNull();

    YCPList result;

    for ( int i=0; i < barGraph->segments(); i++ )
	result->add( YCPInteger( barGraph->segment(i).value() ) );

    return result;
}


YCPValue
YCPPropertyHandler::tryGetBarGraphLabels( YWidget * widget )
{
    YBarGraph * barGraph = dynamic_cast<YBarGraph *> (widget);

    if ( ! barGraph )
	return YCPNull();

    YCPList result;

    for ( int i=0; i < barGraph->segments(); i++ )
	result->add( YCPString( barGraph->segment(i).label() ) );

    return result;
}



