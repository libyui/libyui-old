/*
  Copyright (c) [2019] SUSE LLC
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

  File:		YDescribedItem.h

  Author:	Stefan Hundhammer <shundhammer@suse.de>

/-*/

#ifndef YDescribedItem_h
#define YDescribedItem_h

#include "YItem.h"


/**
 * Item class that has a (possibly multi-line) description text in addition to
 * the normal label.
 **/
class YDescribedItem: public YItem
{
public:
    
    /**
     * Constructor with the label, the description and optionally the selected
     * state.
     **/
    YDescribedItem( const std::string & label,
                    const std::string & description,
                    bool                selected = false )
        : YItem( label, selected )
        , _description( description )
        {}

    /**
     * Constructor with the label, the description, the icon name and
     * optionally the selected state.
     **/
    YDescribedItem( const std::string & label,
                    const std::string & description,
                    const std::string & iconName,
                    bool                selected = false )
        : YItem( label, iconName, selected )
        , _description( description )
        {}

    /**
     * Destructor.
     **/
    virtual ~YDescribedItem() {}

    /**
     * Return this item's description text. This is the (typically longer)
     * subtext that the user sees in a dialog, so this will usually be a
     * translated text.
     **/
    std::string description() const { return _description; }

    /**
     * Set this item's description text.
     **/
    void setDescription( const std::string & desc ) { _description = desc; }


private:

    std::string _description;

};      // class YDescribedItem

#endif  // YDescribedItem_h
