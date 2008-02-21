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


#ifndef YCPPropertyHandler_h
#define YCPPropertyHandler_h

#include <ycp/YCPValue.h>
#include <ycp/YCPMap.h>
#include <ycp/YCPTerm.h>
#include <string>
#include "YItem.h"

using std::string;

class YCPItem;
class YWidget;
class YSelectionWidget;


/**
 * Get and set complex widget properties.
 *
 * Simple properties of types string, bool, int are set with
 * YWidget::setProperty() and retrieved with YWidget::getProperty().
 *
 * The functions here are needed for more complex cases, either because there
 * is no unambiguous conversion from a C++ data type to a YCPValue (or vice
 * versa) or for historical reasons to maintain backward compatibility with
 * existing YCP code.
 **/
class YCPPropertyHandler
{
public:
    /**
     * Set a complex property.
     *
     * Return 'true' on success, 'false' on failure.
     **/
    static bool setComplexProperty( YWidget *		widget,
				    const string & 	propertyName,
				    const YCPValue &	val );

    /**
     * Set a complex property from a term (like `Item(`someID) )
     *
     * Return 'true' on success, 'false' on failure.
     **/
    static bool setComplexProperty( YWidget *		widget,
				    const YCPTerm & 	propertyTerm,
				    const YCPValue &	val 		);

    /**
     * Get a complex property.
     *
     * Return YCPNull upon failure, a non-null YCPValue (the result) upon success.
     **/
    static YCPValue getComplexProperty( YWidget * 	widget,
					const string &	propertyName );

    /**
     * Get a complex property from a term (like `Item(`someID) )
     *
     * Return YCPNull upon failure, a non-null YCPValue (the result) upon success.
     **/
    static YCPValue getComplexProperty( YWidget * 	widget,
					const YCPTerm &	propertyTerm );


protected:

    /**
     * All trySet..() functions try to dynamic_cast 'widget' to the expected
     * widget type and then set a property.
     *
     * They all return 'true' upon success and 'false' upon failure.
     **/
    static bool trySetCheckBoxValue			( YWidget * widget, const YCPValue & val );
    static bool trySetSelectionBoxValue			( YWidget * widget, const YCPValue & val );
    static bool trySetTreeValue				( YWidget * widget, const YCPValue & val );
    static bool trySetTableValue			( YWidget * widget, const YCPValue & val );
    static bool trySetDumbTabValue			( YWidget * widget, const YCPValue & val );
    static bool trySetComboBoxValue			( YWidget * widget, const YCPValue & val );
    static bool trySetMenuButtonItems			( YWidget * widget, const YCPValue & val );
    static bool trySetTreeItems				( YWidget * widget, const YCPValue & val );
    static bool trySetTableItems			( YWidget * widget, const YCPValue & val );
    static bool trySetTableCell				( YWidget * widget, const YCPTerm  & propTerm, const YCPValue & val );
    static bool trySetSelectionWidgetItems		( YWidget * widget, const YCPValue & val );
    static bool trySetRadioButtonGroupCurrentButton	( YWidget * widget, const YCPValue & val );
    static bool trySetMultiSelectionBoxSelectedItems	( YWidget * widget, const YCPValue & val );
    static bool trySetMultiSelectionBoxCurrentItem	( YWidget * widget, const YCPValue & val );
    static bool trySetMultiProgressMeterValues		( YWidget * widget, const YCPValue & val );
    static bool trySetBarGraphValues			( YWidget * widget, const YCPValue & val );
    static bool trySetBarGraphLabels			( YWidget * widget, const YCPValue & val );

    /**
     * All trySet..() functions try to dynamic_cast 'widget' to the expected
     * widget type and then retrieve a property.
     *
     * They all return YCPNull upon failure and a non-null YCPValue upon success.
     **/
    static YCPValue tryGetCheckBoxValue			( YWidget * widget );
    static YCPValue tryGetSelectionBoxValue		( YWidget * widget );
    static YCPValue tryGetTreeValue			( YWidget * widget );
    static YCPValue tryGetTableValue			( YWidget * widget );
    static YCPValue tryGetDumbTabValue			( YWidget * widget );
    static YCPValue tryGetComboBoxValue			( YWidget * widget );
    static YCPValue tryGetRadioButtonGroupCurrentButton	( YWidget * widget );
    static YCPValue tryGetMultiSelectionBoxSelectedItems( YWidget * widget );
    static YCPValue tryGetMultiSelectionBoxCurrentItem	( YWidget * widget );
    static YCPValue tryGetTreeOpenItems			( YWidget * widget );
    static YCPValue tryGetTreeCurrentBranch		( YWidget * widget );
    static YCPValue tryGetWizardCurrentItem		( YWidget * widget );
    static YCPValue tryGetTableCell			( YWidget * widget, const YCPTerm & propertyTerm );
    static YCPValue tryGetTableItem			( YWidget * widget, const YCPTerm & propertyTerm );
    static YCPValue tryGetTableItems			( YWidget * widget );
    static YCPValue tryGetTreeItems			( YWidget * widget );
    static YCPValue tryGetMenuButtonItems		( YWidget * widget );
    static YCPValue tryGetSelectionWidgetItems		( YWidget * widget );
    static YCPValue tryGetBarGraphValues		( YWidget * widget );
    static YCPValue tryGetBarGraphLabels		( YWidget * widget );


    /**
     * Helper function for tryGetTreeOpenItems(): Get any open tree items
     * between iterators 'begin' and 'end' and add them to the 'openItems' map.
     **/
    static void getTreeOpenItems( YCPMap & 		openItems,
				  YItemConstIterator 	begin,
				  YItemConstIterator 	end );
};


#endif // YCPPropertyHandler_h
