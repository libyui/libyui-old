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

  File:	      YRpmGroupsTree.h

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YRpmGroupsTree_h
#define YRpmGroupsTree_h


#include "YStringTree.h"


/**
 * Efficient storage for RPM group tags
 **/
class YRpmGroupsTree: public YStringTree
{
public:

    /**
     * Constructor.
     **/

    YRpmGroupsTree();

    /**
     * Destructor.
     **/
    virtual ~YRpmGroupsTree();

    /**
     * Insert an RPM group into this tree if not already present.
     * Splits the RPM group string ("abc/def/ghi") and creates tree items for
     * each level as required.
     * Returns the tree entry for this RPM group.
     **/
    YStringTreeItem * addRpmGroup( const std::string & rpmGroup )
	{ return addBranch( rpmGroup, '/' ); }

    /**
     * Returns the complete (untranslated) RPM group tag string for 'node'.
     **/
    std::string rpmGroup( const YStringTreeItem * node )
	{ return origPath( node, '/', false ); }

    /**
     * Returns the complete translated RPM group tag string for 'node'.
     **/
    std::string translatedRpmGroup( const YStringTreeItem * node )
	{ return translatedPath( node, '/', false ); }

    /**
     * Add a predefined set of RPM groups
     **/
    void addFallbackRpmGroups();
};



#endif // YRpmGroupsTree_h
