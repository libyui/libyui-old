/*************************************************************************************************************

 Copyright (C) 2000 - 2010 Novell, Inc.   All Rights Reserved.

 This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 Public License as published by the Free Software Foundation; either version 2 of the License, or (at your
 option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License along with this program; if not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*************************************************************************************************************/



 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////   __/\\\\\\_____________/\\\__________/\\\________/\\\___/\\\________/\\\___/\\\\\\\\\\\_           ////
 ////    _\////\\\____________\/\\\_________\///\\\____/\\\/___\/\\\_______\/\\\__\/////\\\///__          ////
 ////     ____\/\\\______/\\\__\/\\\___________\///\\\/\\\/_____\/\\\_______\/\\\______\/\\\_____         ////
 ////      ____\/\\\_____\///___\/\\\_____________\///\\\/_______\/\\\_______\/\\\______\/\\\_____        ////
 ////       ____\/\\\______/\\\__\/\\\\\\\\\_________\/\\\________\/\\\_______\/\\\______\/\\\_____       ////
 ////        ____\/\\\_____\/\\\__\/\\\////\\\________\/\\\________\/\\\_______\/\\\______\/\\\_____      ////
 ////         ____\/\\\_____\/\\\__\/\\\__\/\\\________\/\\\________\//\\\______/\\\_______\/\\\_____     ////
 ////          __/\\\\\\\\\__\/\\\__\/\\\\\\\\\_________\/\\\_________\///\\\\\\\\\/_____/\\\\\\\\\\\_    ////
 ////           _\/////////___\///___\/////////__________\///____________\/////////______\///////////__   ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                 widget abstraction library providing Qt, GTK and ncurses frontends                  ////
 ////                                                                                                     ////
 ////                                   3 UIs for the price of one code                                   ////
 ////                                                                                                     ////
 ////                                       ***  core library  ***                                        ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                              (C) SUSE Linux GmbH    ////
 ////                                                                                                     ////
 ////                                                              libYUI-AsciiArt (C) 2012 Björn Esser   ////
 ////                                                                                                     ////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-/

  File:		YWidget.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <signal.h>
#include <iostream>
#include <sstream>

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YShortcut.h"
#include "YWidget.h"
#include "YDialog.h"
#include "YUI.h"
#include "YDialog.h"
#include "YUIException.h"
#include "YWidgetID.h"
#include "YBothDim.h"
#include "YMacroRecorder.h"

#include "YChildrenManager.h"

#define MAX_DEBUG_LABEL_LEN	50
#define YWIDGET_MAGIC		42

#define CHECK_FOR_DUPLICATE_CHILDREN	1
#define LOG_WIDGET_REP			0



struct YWidgetPrivate
{
    /**
     * Constructor
     **/
    YWidgetPrivate( YWidgetChildrenManager * manager, YWidget * parentWidget = 0 )
	: childrenManager( manager )
	, parent( parentWidget )
	, beingDestroyed( false )
	, enabled( true )
	, notify( false )
	, notifyContextMenu( false )
	, sendKeyEvents( false )
	, autoShortcut( false )
	, toolkitWidgetRep( 0 )
	, id( 0 )
	, functionKey( 0 )
    {
	stretch.hor	= false;
	stretch.vert	= false;
	weight.hor	= 0;
	weight.vert	= 0;
    }

    //
    // Data members
    //

    YWidgetChildrenManager *	childrenManager;
    YWidget *			parent;
    bool			beingDestroyed;
    bool			enabled;
    bool			notify;
    bool			notifyContextMenu;
    bool 			sendKeyEvents;
    bool 			autoShortcut;
    void *			toolkitWidgetRep;
    YWidgetID *			id;
    YBothDim<bool>		stretch;
    YBothDim<int>		weight;
    int				functionKey;
    std::string			helpText;
};




bool YWidget::_usedOperatorNew = false;


YWidget::YWidget( YWidget * parent )
    : _magic( YWIDGET_MAGIC )
    , priv( new YWidgetPrivate( new YWidgetChildrenRejector( this ), parent ) )
{
    YUI_CHECK_NEW( priv );
    YUI_CHECK_NEW( priv->childrenManager );

    if ( ! _usedOperatorNew )
    {
	yuiError() << "FATAL: Widget at "
		   << std::hex << (void *) this << std::dec
		   << " not created with operator new !"
		   << std::endl;
	yuiError() << "Check core dump for a backtrace." << std::endl;
	abort();
    }

    _usedOperatorNew = false;

    if ( parent )
	parent->addChild( this );
}


void * YWidget::operator new( size_t size )
{
    _usedOperatorNew = true;
    return ::operator new( size );
}


YWidget::~YWidget()
{
    YUI_CHECK_WIDGET( this );
    setBeingDestroyed();
    // yuiDebug() << "Destructor of YWidget " << this << std::endl;

    deleteChildren();
    YUI::ui()->deleteNotify( this );

    if ( parent() && ! parent()->beingDestroyed() )
	parent()->removeChild( this );

    delete priv->childrenManager;

    if ( priv->id )
	delete priv->id;

    invalidate();
}


YWidgetChildrenManager *
YWidget::childrenManager() const
{
    return priv->childrenManager;
}


void
YWidget::setChildrenManager( YWidgetChildrenManager * newChildrenManager )
{
    YUI_CHECK_PTR( newChildrenManager );

    delete priv->childrenManager;
    priv->childrenManager = newChildrenManager;
}


void
YWidget::addChild( YWidget * child )
{
#if CHECK_FOR_DUPLICATE_CHILDREN
    if ( child && childrenManager()->contains( child ) )
    {
	yuiError() << this << " already contains " << child << std::endl;
	YUI_THROW( YUIInvalidChildException<YWidget>( this, child ) );
    }
#endif

    childrenManager()->add( child );
}


void
YWidget::removeChild( YWidget * child )
{
    if ( ! beingDestroyed() )
    {
	// yuiDebug() << "Removing " << child << " from " << this << std::endl;
	childrenManager()->remove( child );
    }
}


void
YWidget::deleteChildren()
{
    YWidgetList::const_iterator it = childrenBegin();

    while ( it != childrenEnd() )
    {
	YWidget * child = *it;
	++it;

	if ( child->isValid() )
	{
	    // yuiDebug() << "Deleting " << child << std::endl;
	    delete child;
	}
    }

    childrenManager()->clear();
}


std::string
YWidget::debugLabel() const
{
    std::string label = YShortcut::cleanShortcutString( YShortcut::getShortcutString( this ) );

    if ( label.size() > MAX_DEBUG_LABEL_LEN )
    {
	label.resize( MAX_DEBUG_LABEL_LEN );
	label.append( "..." );
    }

    for ( unsigned i=0; i < label.size(); i++ )
    {
	if ( label[i] == '\n' )
	    label[i] = ' ';
    }

    return label;
}


bool
YWidget::isValid() const
{
    return _magic == YWIDGET_MAGIC;
}


void
YWidget::invalidate()
{
    _magic = 0;
}


bool
YWidget::beingDestroyed() const
{
    return priv->beingDestroyed;
}

void
YWidget::setBeingDestroyed()
{
    priv->beingDestroyed = true;
}


YWidget *
YWidget::parent() const
{
    return priv->parent;
}


bool
YWidget::hasParent() const
{
    return priv->parent;
}


void
YWidget::setParent( YWidget * newParent )
{
    if ( newParent && priv->parent )
    {
	YDialog::currentDialog()->dumpWidgetTree();
	yuiWarning() << "Reparenting " << this
		     << " from " << priv->parent
		     << " to " << newParent << std::endl;
	YUI_THROW( YUIException( std::string( widgetClass() ) + " already has a parent!" ) );
    }

    priv->parent = newParent;
}


bool YWidget::sendKeyEvents() const
{
     return priv->sendKeyEvents;
}


void YWidget::setSendKeyEvents( bool doSend )
{
     priv->sendKeyEvents = doSend;
}


bool YWidget::autoShortcut() const
{
     return priv->autoShortcut;
}


void YWidget::setAutoShortcut( bool newAutoShortcut )
{
     priv->autoShortcut = newAutoShortcut;
}


int YWidget::functionKey() const
{
    return priv->functionKey;
}


bool YWidget::hasFunctionKey() const
{
    return priv->functionKey > 0;
}


void YWidget::setFunctionKey( int fkey_no )
{
    priv->functionKey = fkey_no;
}


std::string YWidget::helpText() const
{
    return priv->helpText;
}


void YWidget::setHelpText( const std::string & helpText )
{
    priv->helpText = helpText;
}


YWidgetID *
YWidget::id() const
{
    return priv->id;
}


void YWidget::setId( YWidgetID * newId )
{
    if ( priv->id )
	delete priv->id;

    priv->id = newId;
}


bool YWidget::hasId() const
{
    return priv->id != 0;
}


YDialog * YWidget::findDialog()
{
    YWidget * widget = this;

    while ( widget )
    {
	YDialog * dialog = dynamic_cast<YDialog *> (widget);

	if ( dialog )
	    return dialog;
	else
	    widget = widget->parent();
    }

    return 0;
}


const YPropertySet &
YWidget::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/**
	 * @property boolean		Enabled		enabled/disabled state of this widget
	 * @property boolean		Notify		the current notify state (see also `opt( `notify ))
	 * @property boolean		ContextMenu	the current contextmenu state (see also `opt( `notifyContextMenu ))
	 * @property std::string	WidgetClass	the widget class of this widget (YLabel, YPushButton, ...)
	 * @property std::string	DebugLabel	(possibly translated) text describing this widget for debugging
	 * @property std::string	HelpText	help text
	 * @property integer		HWeight		horizontal layout weight (same as `HWeight(widget())
	 * @property integer		VWeight		vertical   layout weight (same as `VWeight(widget())
	 * @property boolean		HStretch	horizontally stretchable? (same as `opt(`hstretch))
	 * @property boolean		VStretch	vertically   stretchable? (same as `opt(`vstretch))
	 **/

	propSet.add( YProperty( YUIProperty_Enabled,		YBoolProperty	 ) );
	propSet.add( YProperty( YUIProperty_Notify,		YBoolProperty	 ) );
	propSet.add( YProperty( YUIProperty_WidgetClass,	YStringProperty, true	) ); // read-only
	propSet.add( YProperty( YUIProperty_DebugLabel,		YStringProperty, true	) ); // read-only
	propSet.add( YProperty( YUIProperty_HelpText,		YStringProperty  ) );
	propSet.add( YProperty( YUIProperty_HWeight,		YIntegerProperty ) );
	propSet.add( YProperty( YUIProperty_VWeight,		YIntegerProperty ) );
	propSet.add( YProperty( YUIProperty_HStretch,		YBoolProperty    ) );
	propSet.add( YProperty( YUIProperty_VStretch,		YBoolProperty    ) );
    }

    return propSet;
}


bool
YWidget::setProperty( const std::string & propertyName, const YPropertyValue & val )
{
    try
    {
	propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch
    }
    catch( YUIPropertyException & exception )
    {
	exception.setWidget( this );
	throw;
    }

    if      ( propertyName == YUIProperty_Enabled  )	setEnabled( val.boolVal() );
    else if ( propertyName == YUIProperty_Notify   )	setNotify ( val.boolVal() );
    else if ( propertyName == YUIProperty_HelpText )	setHelpText( val.stringVal() );
    else if ( propertyName == YUIProperty_HWeight  )	setWeight( YD_HORIZ, val.integerVal() );
    else if ( propertyName == YUIProperty_VWeight  )	setWeight( YD_VERT , val.integerVal() );
    else if ( propertyName == YUIProperty_HStretch )	setStretchable( YD_HORIZ, val.boolVal() );
    else if ( propertyName == YUIProperty_VStretch )	setStretchable( YD_VERT , val.boolVal() );

    return true; // success -- no special processing necessary
}


YPropertyValue
YWidget::getProperty( const std::string & propertyName )
{
    try
    {
	propertySet().check( propertyName ); // throws exceptions if not found
    }
    catch( YUIPropertyException & exception )
    {
	exception.setWidget( this );
	throw;
    }

    if ( propertyName == YUIProperty_Enabled 		) return YPropertyValue( isEnabled() 	);
    if ( propertyName == YUIProperty_Notify  		) return YPropertyValue( notify()   	);
    if ( propertyName == YUIProperty_ContextMenu	) return YPropertyValue( notifyContextMenu() );
    if ( propertyName == YUIProperty_WidgetClass	) return YPropertyValue( widgetClass() 	);
    if ( propertyName == YUIProperty_HelpText		) return YPropertyValue( helpText() 	);
    if ( propertyName == YUIProperty_DebugLabel		) return YPropertyValue( debugLabel()	);
    if ( propertyName == YUIProperty_HWeight		) return YPropertyValue( weight( YD_HORIZ ) );
    if ( propertyName == YUIProperty_VWeight		) return YPropertyValue( weight( YD_VERT  ) );
    if ( propertyName == YUIProperty_HStretch		) return YPropertyValue( stretchable( YD_HORIZ ) );
    if ( propertyName == YUIProperty_VStretch		) return YPropertyValue( stretchable( YD_VERT  ) );

    return YPropertyValue( false ); // NOTREACHED
}


void *
YWidget::widgetRep() const
{
    return priv->toolkitWidgetRep;
}


void
YWidget::setWidgetRep( void * rep )
{
    priv->toolkitWidgetRep = rep;
}


void
YWidget::setEnabled( bool enabled )
{
    priv->enabled = enabled;
}


bool
YWidget::isEnabled() const
{
    return priv->enabled;
}


void YWidget::setShortcutString( const std::string & str )
{
    yuiError() << "Default setShortcutString() method called - "
	       << "this should be reimplemented in "
	       << widgetClass()
	       << std::endl;
}


void YWidget::setNotify( bool notify )
{
    priv->notify = notify;
}


void YWidget::setNotifyContextMenu( bool notifyContextMenu )
{
    priv->notifyContextMenu = notifyContextMenu;
}


bool YWidget::notify() const
{
    return priv->notify;
}


bool YWidget::notifyContextMenu() const
{
    return priv->notifyContextMenu;
}


int YWidget::preferredSize( YUIDimension dim )
{
    switch ( dim )
    {
	case YD_HORIZ:	return preferredWidth();
	case YD_VERT :	return preferredHeight();

	default:
	    YUI_THROW( YUIInvalidDimensionException() );
	    return 0;
    }
}


void YWidget::setStretchable( YUIDimension dim, bool newStretch )
{
    priv->stretch[ dim ] = newStretch;
}


void YWidget::setDefaultStretchable( YUIDimension dim, bool newStretch )
{
    priv->stretch[ dim ] |= newStretch;
}


bool YWidget::stretchable( YUIDimension dim ) const
{
    return priv->stretch[ dim ];
}


int YWidget::weight( YUIDimension dim )
{
    return priv->weight[ dim ];
}


void YWidget::setWeight( YUIDimension dim, int weight )
{
    priv->weight[ dim ] = weight;
}


bool YWidget::hasWeight( YUIDimension dim )
{
    // DO NOT simply return priv->weight[ dim ] here
    // since weight() might be overwritten in derived classes!

    return weight( dim ) > 0;
}


bool YWidget::setKeyboardFocus()
{
    yuiWarning() << this << " cannot accept the keyboard focus." << std::endl;
    return false;
}


YWidget *
YWidget::findWidget( YWidgetID * id, bool doThrow ) const
{
    if ( ! id )
    {
	if ( doThrow )
	    YUI_THROW( YUIWidgetNotFoundException( "Null ID" ) );

	return 0;
    }

    for ( YWidgetListConstIterator it = childrenBegin();
	  it != childrenEnd();
	  ++it )
    {
	YWidget * child = *it;
	YUI_CHECK_WIDGET( child );

	if ( child->id() && child->id()->isEqual( id ) )
	    return child;

	if ( child->hasChildren() )
	{
	    YWidget * found = child->findWidget( id, false );

	    if ( found )
		return found;
	}
    }

    if ( doThrow )
	YUI_THROW( YUIWidgetNotFoundException( id->toString() ) );

    return 0;
}


void YWidget::setChildrenEnabled( bool enabled )
{
    for ( YWidgetListConstIterator it = childrenBegin();
	  it != childrenEnd();
	  ++it )
    {
	YWidget * child = *it;

	if ( child->hasChildren() )
	{
	    // yuiDebug() << "Recursing into " << child << std::endl;
	    child->setChildrenEnabled( enabled );
	}

	// yuiDebug() << ( enabled ? "Enabling " : "Disabling " ) << child << std::endl;
	child->setEnabled( enabled );
    }
}


void YWidget::dumpDialogWidgetTree()
{
    YWidget * dialog = findDialog();

    if ( dialog )
	dialog->dumpWidgetTree();
    else
	dumpWidgetTree();
}


void YWidget::dumpWidgetTree( int indentationLevel )
{
    dumpWidget( this, indentationLevel );

    for ( YWidgetListConstIterator it = childrenBegin();
	  it != childrenEnd();
	  ++it )
    {
	YWidget * child = *it;

	if ( child->hasChildren() )
	    child->dumpWidgetTree ( indentationLevel + 1 );
	else
	    dumpWidget( child, indentationLevel + 1 );
    }
}


void YWidget::dumpWidget( YWidget *w, int indentationLevel )
{
    std::ostringstream str;

    std::string indentation ( indentationLevel * 4, ' ' );
    str << "Widget tree: " << indentation << w;

    if ( w->widgetRep() )
    {
	str << " (widgetRep: "
	    << std::hex << w->widgetRep() << std::dec
	    << ")";
    }

    std::string stretch;

    if ( w->stretchable( YD_HORIZ ) )	stretch += "hstretch ";
    if ( w->stretchable( YD_VERT  ) )	stretch += "vstretch";

    if ( ! stretch.empty() )
	str << " ( " << stretch << " ) ";

    yuiMilestone() << str.str() << std::endl;
}


void
YWidget::saveUserInput( YMacroRecorder *macroRecorder )
{
    //
    // Record this widget's user input property (if there is any)
    //

    if ( userInputProperty() )
    {
	macroRecorder->recordWidgetProperty( this, userInputProperty() );
    }

    //
    // Record the child widgets' (if there are any) user input
    //

    for ( YWidgetListConstIterator it = childrenBegin();
	  it != childrenEnd();
	  ++it )
    {
	YWidget *widget = *it;

	if ( widget->hasChildren() || widget->hasId() )
	{
	    /*
	     * It wouldn't do any good to save the user input of any widget
	     * that doesn't have an ID since this ID is required to make use of
	     * this saved data later when playing the macro.
	     * Other than that, container widgets need to recurse over all
	     * their children.
	     */

	    widget->saveUserInput( macroRecorder );
	}
    }
}


std::ostream & operator<<( std::ostream & stream, const YWidget * w )
{
    if ( w )
    {
	stream << w->widgetClass();

	std::string debugLabel = w->debugLabel();

	if ( debugLabel.empty() )
	{
	    if ( w->hasId() )
		stream << " ID: \"" << w->id() << "\"";
	}
	else	// Has debugLabel
	{
	    stream << " \"" << debugLabel << "\"";
	}

	stream << " at " << std::hex << (void *) w << std::dec;

#if LOG_WIDGET_REP
	if ( w->widgetRep() )
	{
	    stream << " (widgetRep: "
		   << std::hex << w->widgetRep() << std::dec
		   << ")";
	}
#endif
    }
    else
    {
	stream << "<NULL widget>";
    }

    return stream;
}
