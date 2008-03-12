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

  File:		YTree.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YTree_h
#define YTree_h

#include "YSelectionWidget.h"

class YTreeItem;
class YTreePrivate;


/**
 * Tree: List box that displays a (scrollable) list of hierarchical items from
 * which the user can select exactly one. Each item has a label text and an
 * optional icon (*).
 *
 * This is very similar to SelectionBox, but each item can have subitems that
 * can be open (expanded) or closed (collapsed).
 *
 * The tree widget also has a caption label that is displayed above the
 * tree. The hotkey displayed in that caption label will move the keyboard
 * focus into the tree item list.
 *
 *
 * (*) Not all UIs (in particular not text-based UIs) support displaying icons,
 * so an icon should never be an exclusive means to display any kind of
 * information.
 **/
class YTree : public YSelectionWidget
{
protected:
    /**
     * Constructor.
     **/
    YTree( YWidget * parent, const string & label );

public:
    /**
     * Destructor.
     **/
    virtual ~YTree();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YTree"; }

    /**
     * Rebuild the displayed tree from the internally stored YTreeItems.
     *
     * The application should call this (once) after all items have been added
     * with addItem(). YTree::addItems() calls this automatically.
     *
     * Derived classes are required to implement this.
     **/
    virtual void rebuildTree() = 0;

    /**
     * Add multiple items. For some UIs, this can be more efficient than
     * calling addItem() multiple times. This function also automatically calls
     * rebuildTree() at the end.
     *
     * Derived classes can overwrite this function, but they should call this
     * base class function at the end of the new implementation.
     *
     * Reimplemented from YSelectionWidget.
     **/
    virtual void addItems( const YItemCollection & itemCollection );

    /**
     * Deliver even more events than with notify() set.
     *
     * For YTree, this is relevant mostly for the NCurses UI:
     *
     * In graphical UIs like the Qt UI, the user can use the mouse to select an
     * item in a tree. With notify() set, this will send an event right away
     * (i.e., it will make UserInput and related return, while normally it
     * would only return when the user clicks a PushButton).
     *
     * In the NCurses UI, there is no mouse, so the user has to use the cursor
     * keys to move to the item he wants to select. In immediateMode(), every
     * cursor key press will make the tree send an event. Without
     * immediateMode(), the NCTree will wait until the user hits the [Return]
     * key until an event is sent. Depending on what the application does upon
     * each selection box event, immediateMode() might make the application
     * less responsive.
     **/
    bool immediateMode() const;

    /**
     * Set immediateMode() on or off.
     **/
    void setImmediateMode( bool on = true );

    /**
     * Set a property.
     * Reimplemented from YWidget.
     *
     * This function may throw YUIPropertyExceptions.
     *
     * This function returns 'true' if the value was successfully set and
     * 'false' if that value requires special handling (not in error cases:
     * those are covered by exceptions).
     **/
    virtual bool setProperty( const string & propertyName,
			      const YPropertyValue & val );

    /**
     * Get a property.
     * Reimplemented from YWidget.
     *
     * This method may throw YUIPropertyExceptions.
     **/
    virtual YPropertyValue getProperty( const string & propertyName );

    /**
     * Return this class's property set.
     * This also initializes the property upon the first call.
     *
     * Reimplemented from YWidget.
     **/
    virtual const YPropertySet & propertySet();

    /**
     * The name of the widget property that will return user input.
     * Inherited from YWidget.
     **/
    const char * userInputProperty() { return YUIProperty_CurrentItem; }

private:

    ImplPtr<YTreePrivate> priv;
};


#endif // YTree_h
