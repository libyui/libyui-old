/*
  |****************************************************************************
  |
  | Copyright (c) 2000 - 2012 Novell, Inc.
  | All Rights Reserved.
  |
  | This program is free software; you can redistribute it and/or
  | modify it under the terms of version 2 of the GNU General Public License as
  | published by the Free Software Foundation.
  |
  | This program is distributed in the hope that it will be useful,
  | but WITHOUT ANY WARRANTY; without even the implied warranty of
  | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
  | GNU General Public License for more details.
  |
  | You should have received a copy of the GNU General Public License
  | along with this program; if not, contact Novell, Inc.
  |
  | To contact Novell about this file by physical or electronic mail,
  | you may find current contact information at www.novell.com
  |
  |****************************************************************************
*/


/*-/

  File:		YDateField.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YDateField_h
#define YDateField_h

#include "YSimpleInputField.h"

class YDateFieldPrivate;

/**
 * Input field for entering a date.
 *
 * Derived classes are required to implement:
 *     	value()
 *	setValue()
 *
 * For both methods the date is formatted as "YYYY-MM-DD".
 * See YSimpleInputField.h for more details.
 **/
class YDateField : public YSimpleInputField
{
protected:
    /**
     * Constructor.
     **/
    YDateField( YWidget * parent, const std::string & label );

public:
    /**
     * Destructor.
     **/
    virtual ~YDateField();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YDateField"; }


private:

    ImplPtr<YDateFieldPrivate> priv;
};


#endif // YDateField_h
