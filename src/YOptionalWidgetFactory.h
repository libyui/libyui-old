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

  File:		YOptionalWidgetFactory.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YOptionalWidgetFactory_h
#define YOptionalWidgetFactory_h

#include <string>
#include <vector>
#include <map>

#include "YTypes.h"
#include "YWizard.h"

using std::string;
using std::vector;
using std::map;

class YBarGraph;
class YDateField;
class YDownloadProgress;
class YDumbTab;
class YDummySpecialWidget;
class YMultiProgressMeter;
class YPartitionSplitter;
class YSlider;
class YTimeField;
class YWidget;
class YTimezoneSelector;

/**
 * Abstract widget factory for optional ("special") widgets.
 *
 * Remember to always check with the corresponding "has..()" method if the
 * current UI actually provides the requested widget. Otherwise the
 * "create...()" method will throw an exception.
 **/
class YOptionalWidgetFactory
{
public:

    //
    // Optional Widgets
    //

    virtual bool			hasWizard();
    virtual YWizard *			createWizard		( YWidget * 		parent,
								  const string & 	backButtonLabel,
								  const string & 	abortButtonLabel,
								  const string & 	nextButtonLabel,
								  YWizardMode		wizardMode = YWizardMode_Standard );

    virtual bool			hasDumbTab();
    virtual YDumbTab *			createDumbTab		( YWidget * parent );

    virtual bool			hasSlider();
    virtual YSlider *			createSlider		( YWidget *		parent,
								  const string	&	label,
								  int 			minVal,
								  int 			maxVal,
								  int 			initialVal );

    virtual bool			hasDateField();
    virtual YDateField *		createDateField		( YWidget * parent, const string & label );

    virtual bool			hasTimeField();
    virtual YTimeField *		createTimeField		( YWidget * parent, const string & label );

    virtual bool			hasBarGraph();
    virtual YBarGraph *			createBarGraph		( YWidget * parent );

    virtual bool			hasPatternSelector();
    virtual YWidget *			createPatternSelector	( YWidget * parent, long modeFlags = 0 );

    virtual bool			hasSimplePatchSelector();
    virtual YWidget *			createSimplePatchSelector( YWidget * parent, long modeFlags = 0 );

    virtual bool			hasMultiProgressMeter();
    YMultiProgressMeter *		createHMultiProgressMeter( YWidget * parent, const vector<float> & maxValues );
    YMultiProgressMeter *		createVMultiProgressMeter( YWidget * parent, const vector<float> & maxValues );
    virtual YMultiProgressMeter *	createMultiProgressMeter ( YWidget * parent, YUIDimension dim, const vector<float> & maxValues );

    virtual bool			hasPartitionSplitter();
    virtual YPartitionSplitter *	createPartitionSplitter	( YWidget * 		parent,
								  int 			usedSize,
								  int 			totalFreeSize,
								  int 			newPartSize,
								  int 			minNewPartSize,
								  int 			minFreeSize,
								  const string &	usedLabel,
								  const string &	freeLabel,
								  const string &	newPartLabel,
								  const string &	freeFieldLabel,
								  const string &	newPartFieldLabel );


    virtual bool			hasDownloadProgress();
    virtual YDownloadProgress *		createDownloadProgress	( YWidget * 		parent,
								  const string & 	label,
								  const string & 	filename,
								  YFileSize_t		expectedFileSize );

    bool				hasDummySpecialWidget();
    YWidget *				createDummySpecialWidget( YWidget * parent );

    virtual bool                        hasTimezoneSelector();
    virtual YTimezoneSelector *         createTimezoneSelector( YWidget * parent,
								const string & pixmap, 
								const map<string,string> & timezones );

protected:

    friend class YUI;

    /**
     * Constructor.
     *
     * Use YUI::optionalWidgetFactory() to get the singleton for this class.
     **/
    YOptionalWidgetFactory();

    /**
     * Destructor.
     **/
    virtual ~YOptionalWidgetFactory();

}; // class YOptionalWidgetFactory



#endif // YOptionalWidgetFactory_h
