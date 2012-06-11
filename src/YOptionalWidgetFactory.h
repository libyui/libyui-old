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
#include "YGraph.h"


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
								  const std::string & 	backButtonLabel,
								  const std::string & 	abortButtonLabel,
								  const std::string & 	nextButtonLabel,
								  YWizardMode		wizardMode = YWizardMode_Standard );

    virtual bool			hasDumbTab();
    virtual YDumbTab *			createDumbTab		( YWidget * parent );

    virtual bool			hasSlider();
    virtual YSlider *			createSlider		( YWidget *		parent,
								  const std::string &	label,
								  int 			minVal,
								  int 			maxVal,
								  int 			initialVal );

    virtual bool			hasDateField();
    virtual YDateField *		createDateField		( YWidget * parent, const std::string & label );

    virtual bool			hasTimeField();
    virtual YTimeField *		createTimeField		( YWidget * parent, const std::string & label );

    virtual bool			hasBarGraph();
    virtual YBarGraph *			createBarGraph		( YWidget * parent );

    virtual bool			hasPatternSelector();
    virtual YWidget *			createPatternSelector	( YWidget * parent, long modeFlags = 0 );

    virtual bool			hasSimplePatchSelector();
    virtual YWidget *			createSimplePatchSelector( YWidget * parent, long modeFlags = 0 );

    virtual bool			hasMultiProgressMeter();
    YMultiProgressMeter *		createHMultiProgressMeter( YWidget * parent, const std::vector<float> & maxValues );
    YMultiProgressMeter *		createVMultiProgressMeter( YWidget * parent, const std::vector<float> & maxValues );
    virtual YMultiProgressMeter *	createMultiProgressMeter ( YWidget * parent, YUIDimension dim, const std::vector<float> & maxValues );

    virtual bool			hasPartitionSplitter();
    virtual YPartitionSplitter *	createPartitionSplitter	( YWidget * 		parent,
								  int 			usedSize,
								  int 			totalFreeSize,
								  int 			newPartSize,
								  int 			minNewPartSize,
								  int 			minFreeSize,
								  const std::string &	usedLabel,
								  const std::string &	freeLabel,
								  const std::string &	newPartLabel,
								  const std::string &	freeFieldLabel,
								  const std::string &	newPartFieldLabel );


    virtual bool			hasDownloadProgress();
    virtual YDownloadProgress *		createDownloadProgress	( YWidget * 		parent,
								  const std::string & 	label,
								  const std::string & 	filename,
								  YFileSize_t		expectedFileSize );

    bool				hasDummySpecialWidget();
    YWidget *				createDummySpecialWidget( YWidget * parent );

    virtual bool                        hasTimezoneSelector();
    virtual YTimezoneSelector *         createTimezoneSelector( YWidget * parent,
								const std::string & pixmap,
								const std::map<std::string,std::string> & timezones );

    virtual bool			hasGraph();
    virtual YGraph *			createGraph( YWidget * parent, const std::string & filename,
						     const std::string & layoutAlgorithm );
    virtual YGraph *			createGraph( YWidget * parent, graph_t * graph );

    virtual bool			hasContextMenu();

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
