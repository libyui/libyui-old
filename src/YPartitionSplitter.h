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

  File:		YPartitionSplitter.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YPartitionSplitter_h
#define YPartitionSplitter_h

#include "YWidget.h"


class YPartitionSplitterPrivate;


/**
 * PartitionSplitter: A (very custom) widget for easily splitting one existing
 * partition into two.
 *
 * Layout:
 *
 *     +--------------------+---------------+--------------------------+
 *     | Old Partition      | Old Partition | New Partition            |
 *     | used               | free          |                          |
 *     +--------------------+---------------+--------------------------+
 *
 *     Old Partition free                                  New Partition
 *     [ 123 ] ================O================================ [ 123 ]
 *
 *
 * At the top, there is a BarGraph that dynamicylla displays the sizes in
 * graphical form. Below are an IntField to the left and an IntField to the
 * right, each with its respective label. Between the two IntFields there is a
 * Slider.
 *
 * The user can enter a value in either IntField or drag the slider. The other
 * sub-widgets (including the BarGraph) will automatically be
 * adjusted. Visually (in the BarGraph), the border between "old partition
 * free" and "new partition" will move left and right. The border between "old
 * partition used" and "old partition free" is static.
 *
 * There are built-in (configurable) limits for the minimum sizes of "old
 * partition free" and "new partition".
 **/
class YPartitionSplitter : public YWidget
{
protected:
    /**
     * Constructor.
     *
     * usedSize:	Used size of the old partition (constant)
     *
     * totalFreeSize:	Total free size of the old partition before the split:
     *			OldPartitionFree + NewPartition
     *
     * newPartSize':	Initial size of the new partition
     *
     * minNewPartSize:	Miminum size of the new partition
     *
     * minFreeSize:	Minimum free size of the old partition
     *
     * usedLabel:	BarGraph label for the used part of the old partition
     *
     * freeLabel:	BarGraph label for the free part of the old partition
     *
     * newPartLabel:	BarGraph label for the new partition
     *
     * freeFieldLabel:	IntField label for the free part of the old partition
     *
     * newPartFieldLabel: IntField label for the size of the new partition
     **/
    YPartitionSplitter( YWidget *	parent,
			int 		usedSize,
			int 		totalFreeSize,
			int 		newPartSize,
			int 		minNewPartSize,
			int 		minFreeSize,
			const string &	usedLabel,
			const string &	freeLabel,
			const string &	newPartLabel,
			const string &	freeFieldLabel,
			const string &	newPartFieldLabel );

public:

    /**
     * Destructor.
     **/
    virtual ~YPartitionSplitter();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YPartitionSplitter"; }

    /**
     * The value of this PartitionSplitter: The size of the new partition.
     *
     * Derived classes are required to implement this.
     **/
    virtual int value() = 0;

    /**
     * Set the value (the size of the new partition).
     *
     * Derived classes are required to implement this.
     **/
    virtual void setValue( int newValue ) = 0;


    // Access methods

    int 	usedSize()         	const;
    int 	totalFreeSize()    	const;
    int 	minFreeSize()        	const;
    int 	maxFreeSize()        	const	{ return totalFreeSize() - minNewPartSize(); }
    int		freeSize()			{ return totalFreeSize() - newPartSize(); }
    int 	newPartSize()			{ return value(); }
    int 	minNewPartSize()	const;
    int		maxNewPartSize()	const	{ return totalFreeSize() - minFreeSize(); }

    string	usedLabel()          	const;
    string 	freeLabel()          	const;
    string 	newPartLabel()         	const;
    string 	freeFieldLabel()     	const;
    string 	newPartFieldLabel()    	const;

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
    const char * userInputProperty() { return YUIProperty_Value; }


private:

    ImplPtr<YPartitionSplitterPrivate> priv;
};


#endif // YPartitionSplitter_h
