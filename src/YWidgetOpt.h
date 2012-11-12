/****************************************************************************

Copyright (c) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, contact Novell, Inc.

To contact Novell about this file by physical or electronic mail,
you may find current contact information at www.novell.com

****************************************************************************

  File:		YWidgetOpt.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YWidgetOpt_h
#define YWidgetOpt_h


/**
 * Helper template class for options that may have some value or may be undefined
 **/
template<class T> class YAnyOpt
{
public:
    /**
     * Constructor
     **/
    YAnyOpt()			{ _defined = false; }

    /**
     * Destructor
     **/
    virtual ~YAnyOpt() {}

    /**
     * Set the value.
     **/
    void setValue( T newValue )	{ _defined = true; _value = newValue; }

    /**
     * Invalidate the value, i.e. make it undefined - just like at
     * object creation time.
     **/
    void undef()		{ _defined = false;	}

    /**
     * Return whether or not the value is defined, i.e. whether or not
     * a value has been set before.
     **/
    bool defined() const	{ return _defined;	}

    /**
     * Returns the value of this option, if any has been defined,
     * or the default value otherwise.
     **/
    T value() const		{ return _defined ? _value : defaultValue(); }

protected:

    /**
     * The default value - the value that will be returned if none has
     * been explicitly set.
     * Overwrite this method with the class specific default value.
     **/
    virtual T defaultValue() const = 0;

    bool _defined;
    T	_value;
};


/**
 * boolean option - tri state: true, false, undefined
 **/
class YBoolOpt: public YAnyOpt<bool>
{
    virtual bool defaultValue() const { return false; }
};


/**
 * long option - long value or undefined
 **/
class YLongOpt: public YAnyOpt<long>
{
    virtual long defaultValue() const { return 0L; }
};


/**
 * collection of various widget options to be passed
 * transparently through the various library layers.
 *
 * This is a struct and not an object since there is neither a
 * constructor that would make much sense nor a destructor: The data
 * members are designed to initialize themselves properly, and we
 * might as well use their well-defined access methods rather than
 * create another layer of wrappers which would just add to overhead
 * and confusion.
 *
 * Just never use anything other than YAnyOpt and classes derived from
 * it here, and everything will be fine.
 *
 * This struct is transparently passed from
 * YUIInterpreter::createWidget() through the
 * YUInterpreter::create???() and Y??UI::create???() methods down to
 * the YWidget derived constructors and the YWidget constructor
 * itself.  The general idea is to have a container for such widget
 * options from where any of those layers can read whatever they are
 * interested in, yet ignore what they don't know or don't want.
 **/
struct YWidgetOpt
{
    // Common options for all widgets.
    // See the inline doc in YCPDialogParser for details.

    YBoolOpt isDisabled;
    YBoolOpt notifyMode;
    YBoolOpt notifyContextMenu;
    YBoolOpt isHStretchable;
    YBoolOpt isVStretchable;
    YBoolOpt autoShortcut;
    YBoolOpt keyEvents;


    // Widget-specific options
    //
    // See the respective widget doc in YCPDialogParser::parse???()

    YBoolOpt boldFont;		// YCheckBox, YRadioButton, YLabel
    YLongOpt key_Fxx;		// YPushButton: No. of F-Key (1..24), 0 if none
    YBoolOpt customButton;	// YPushButton: Override other button roles
};


#endif // YWidgetOpt_h
