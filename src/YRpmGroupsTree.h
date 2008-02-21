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
