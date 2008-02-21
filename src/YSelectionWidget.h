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

  File:		YSelectionWidget.h

  Author:     	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YSelectionWidget_h
#define YSelectionWidget_h

#include "YWidget.h"
#include "YItem.h"
#include "ImplPtr.h"

using std::string;
class YSelectionWidgetPrivate;

/**
 * Base class for selection widgets:
 *   - YSelectionBox
 *   - MultiselectionBox
 *   - YCombobox
 *   - YTree
 *   - YDumbTab
 **/
class YSelectionWidget : public YWidget
{
protected:

    /**
     * Constructor.
     *
     * 'singleSelectionMode' indicates if this base class should enforce single
     * selection when items are added or when items are selected from the
     * application. Note that single selection can also mean that no item is
     * selected.
     **/
    YSelectionWidget( YWidget *		parent,
		      const string & 	label,
		      bool 		enforceSingleSelection );

public:
    /**
     * Destructor.
     **/
    virtual ~YSelectionWidget();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YSelectionWidget"; }

    /**
     * Return this widget's label (the caption above the item list).
     **/
    string label() const;

    /**
     * Change this widget's label (the caption above the item list).
     *
     * Derived classes should overwrite this function, but they should call
     * this base class function in the new implementation.
     **/
    virtual void setLabel( const string & newLabel );

    /**
     * Add one item. This widget assumes ownership of the item object and will
     * delete it in its destructor.
     *
     * NOTE: For tree items, call this only for the toplevel items; all
     * non-toplevel items are already owned by their respective parent
     * items. Adding them to the parent widget will clash with this ownership.
     *
     * Derived classes can overwrite this function, but they should call this
     * base class function in the new implementation.
     **/
    virtual void addItem( YItem * item );

    /**
     * Overloaded for convenience: Add an item by string.
     **/
    void addItem( const string & itemLabel, bool selected = false );

    /**
     * Overloaded for convenience: Add an item with a text and an icon.
     * Note that not all UIs can display icons.
     **/
    void addItem( const string & itemLabel,
		  const string & iconName,
		  bool selected = false );

    /**
     * Add multiple items. For some UIs, this can be more efficient than
     * calling addItem() multiple times.
     *
     * Derived classes can overwrite this function, but they should call this
     * base class function in the new implementation.
     **/
    virtual void addItems( const YItemCollection & itemCollection );

    /**
     * Delete all items.
     *
     * Derived classes can overwrite this function, but they should call this
     * base class function in the new implementation.
     **/
    virtual void deleteAllItems();

    /**
     * Delete all items and add new items.
     **/
    void setItems( const YItemCollection & itemCollection )
	{ deleteAllItems(); addItems( itemCollection ); }

    /**
     * Return an iterator that points to the first item.
     *
     * For YSelectionWidgets that can have tree structures, this iterator will
     * iterate over the toplevel items.
     *
     * Important: Don't use this iterator to iterate over all items and check
     * their "selected" state; that information might not always be up to
     * date. Use the dedicated functions for that.
     **/
    YItemIterator	itemsBegin();
    YItemConstIterator	itemsBegin() const;

    /**
     * Return an iterator that points behind the last item.
     **/
    YItemIterator	itemsEnd();
    YItemConstIterator	itemsEnd() const;

    /**
     * Return 'true' if this widget has any items.
     **/
    bool hasItems() const;

    /**
     * Return the number of items.
     *
     * For YSelectionWidgets that can have tree structures, this returns the
     * number of toplevel items.
     **/
    int itemsCount() const;

    /**
     * Return the first item or 0 if there is none.
     **/
    YItem * firstItem() const;

    /**
     * Return the (first) selected item or 0 if none is selected.
     **/
    virtual YItem * selectedItem();

    /**
     * Return all selected items. This is mostly useful for derived classes
     * that allow selecting multiple items.
     *
     * This function does not transfer ownership of those items to the caller,
     * so don't try to delete them!
     **/
    virtual YItemCollection selectedItems();

    /**
     * Return 'true' if any item is selected.
     **/
    bool hasSelectedItem();

    /**
     * Select or deselect an item.
     *
     * Notice that this is different from YItem::setSelected() because unlike
     * the latter function, this function informs the parent widget of the
     * selection change.
     *
     * If only one item can be selected at any time (single selection), the
     * derived class will make sure to deselect any previous selection, if
     * applicable.
     *
     * Derived classes should overwrite this function, but they should call
     * this base class function at the new function's start (this will also
     * check if the item really belongs to this widget and throw an exception
     * if not).
     **/
    virtual void selectItem( YItem * item, bool selected = true );

    /**
     * Deselect all items.
     *
     * Derived classes can overwrite this function, but they should call this
     * base class function in the new implementation.
     **/
    virtual void deselectAllItems();

    /**
     * Set this widget's base path where to look up icons.
     * If this is a relative path, YUI::qApp()->iconBasePath() is prepended.
     **/
    void setIconBasePath( const string & basePath );

    /**
     * Return this widget's base path where to look up icons
     * as set with setIconBasePath().
     **/
    string iconBasePath() const;

    /**
     * Return the full path + file name for the specified icon name.
     *
     * If 'iconName' is empty, this will return an empty string.
     **/
    string iconFullPath( const string & iconName ) const;
    
    /**
     * Return the full path + file name for the icon of the specified item.
     *
     * If 'item' does not have an iconName specified, this will return an empty
     * string.
     **/
    string iconFullPath( YItem * item ) const;

    /**
     * Return 'true' if this widget's items contain the specified item.
     **/
    bool itemsContain( YItem * item ) const;

    /**
     * Find the (first) item with the specified label.
     * Return 0 if there is no item with that label.
     **/
    YItem * findItem( const string & itemLabel ) const;

    /**
     * Get the string of this widget that holds the keyboard shortcut.
     *
     * Reimplemented from YWidget.
     **/
    virtual string shortcutString() const { return label(); }

    /**
     * Set the string of this widget that holds the keyboard shortcut.
     *
     * Reimplemented from YWidget.
     **/
    virtual void setShortcutString( const string & str )
	{ setLabel( str ); }

protected:

    /**
     * Set single selection mode on or off. In single selection mode, only one
     * item can be selected at any time.
     *
     * If set, this base class enforces this when items are added or when items
     * are selected from the application. Note that single selection can also
     * mean that no item is selected.
     **/
    void setEnforceSingleSelection( bool on );

    /**
     * Return 'true' if this base class should enforce single selection.
     **/
    bool enforceSingleSelection() const;

    /**
     * Recursively try to find the first selected item between iterators
     * 'begin' and 'end'. Return that item or 0 if there is none.
     **/
    YItem * findSelectedItem( YItemConstIterator begin,
			      YItemConstIterator end );

    /**
     * Recursively find all selected items between iterators 'begin' and 'end'
     * and add each of them to the 'selectedItems' YItemCollection.
     **/
    void findSelectedItems( YItemCollection &	selectedItems,
			    YItemConstIterator	begin,
			    YItemConstIterator	end );

    /**
     * Recursively deselect all items between iterators 'begin' and 'end'.
     **/
    void deselectAllItems( YItemIterator	begin,
			   YItemIterator	end );
    /**
     * Recursively try to find an item with label 'wantedItemLabel' between
     * iterators 'begin' and 'end'. Return that item or 0 if there is none.
     **/
    YItem * findItem	( const string &	wantedItemLabel,
			  YItemConstIterator	begin,
			  YItemConstIterator	end ) const;

    /**
     * Recursively check if 'wantedItem' is between iterators 'begin' and
     * 'end'.
     **/
    bool itemsContain	( YItem *		wantedItem,
			  YItemConstIterator	begin,
			  YItemConstIterator	end ) const;
    /**
     * Return the item at index 'index' (from 0)
     * or 0 if there is no such item.
     **/
    YItem * itemAt( int index ) const;


private:

    ImplPtr<YSelectionWidgetPrivate> priv;
};


#endif // YSelectionWidget_h
