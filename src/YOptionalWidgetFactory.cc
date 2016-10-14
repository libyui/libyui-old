/*
  Copyright (C) 2000-2012 Novell, Inc
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

  File:		YOptionalWidgetFactory.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include "YOptionalWidgetFactory.h"
#include "YWidgetFactory.h"
#include "YUIException.h"
#include "YLabel.h"
#include "YUI.h"

#define YUILogComponent "owf"
#include "YUILog.h"


#define THROW_UNSUPPORTED( WIDGET_TYPE ) \
    YUI_THROW( YUIUnsupportedWidgetException( WIDGET_TYPE ) );	\
    return 0



YOptionalWidgetFactory::YOptionalWidgetFactory()
{
    // NOP
}

YOptionalWidgetFactory::~YOptionalWidgetFactory()
{
    // NOP
  yuiMilestone() << "YOptionalWidgetFactory removed" << std::endl;

}



bool YOptionalWidgetFactory::hasWizard()
{
    return false;
}

YWizard *
YOptionalWidgetFactory::createWizard( YWidget *			parent,
				      const std::string &	backButtonLabel,
				      const std::string &	abortButtonLabel,
				      const std::string &	nextButtonLabel,
				      YWizardMode		wizardMode )
{
    THROW_UNSUPPORTED( "YWizard" );
}



bool YOptionalWidgetFactory::hasDumbTab()
{
    return false;
}

YDumbTab *
YOptionalWidgetFactory::createDumbTab( YWidget * parent )
{
    THROW_UNSUPPORTED( "YDumbTab" );
}



bool YOptionalWidgetFactory::hasSlider()
{
    return false;
}

YSlider *
YOptionalWidgetFactory::createSlider( YWidget *			parent,
				      const std::string &	label,
				      int			minVal,
				      int			maxVal,
				      int			initialVal )
{
    THROW_UNSUPPORTED( "YSlider" );
}



bool YOptionalWidgetFactory::hasDateField()
{
    return false;
}

YDateField *
YOptionalWidgetFactory::createDateField( YWidget * parent, const std::string & label )
{
    THROW_UNSUPPORTED( "YDateField" );
}



bool YOptionalWidgetFactory::hasTimeField()
{
    return false;
}

YTimeField *
YOptionalWidgetFactory::createTimeField( YWidget * parent, const std::string & label )
{
    THROW_UNSUPPORTED( "YTimeField" );
}



bool YOptionalWidgetFactory::hasBarGraph()
{
    return false;
}

YBarGraph *
YOptionalWidgetFactory::createBarGraph( YWidget * parent )
{
    THROW_UNSUPPORTED( "YBarGraph" );
}



bool YOptionalWidgetFactory::hasPatternSelector()
{
    return false;
}

YWidget *
YOptionalWidgetFactory::createPatternSelector( YWidget * parent, long modeFlags )
{
    THROW_UNSUPPORTED( "YPatternSelector" );
}



bool YOptionalWidgetFactory::hasSimplePatchSelector()
{
    return false;
}

YWidget *
YOptionalWidgetFactory::createSimplePatchSelector( YWidget * parent, long modeFlags )
{
    THROW_UNSUPPORTED( "YSimplePatchSelector" );
}



bool YOptionalWidgetFactory::hasMultiProgressMeter()
{
    return false;
}

YMultiProgressMeter *
YOptionalWidgetFactory::createMultiProgressMeter( YWidget * parent, YUIDimension dim, const std::vector<float> & maxValues )
{
    THROW_UNSUPPORTED( "YMultiProgressMeter" );
}

YMultiProgressMeter *
YOptionalWidgetFactory::createHMultiProgressMeter( YWidget * parent, const std::vector<float> & maxValues )
{
    return createMultiProgressMeter( parent, YD_HORIZ, maxValues );
}

YMultiProgressMeter *
YOptionalWidgetFactory::createVMultiProgressMeter( YWidget * parent, const std::vector<float> & maxValues )
{
    return createMultiProgressMeter( parent, YD_VERT, maxValues );
}



bool YOptionalWidgetFactory::hasPartitionSplitter()
{
    return false;
}

YPartitionSplitter *
YOptionalWidgetFactory::createPartitionSplitter( YWidget *		parent,
						 int			usedSize,
						 int			totalFreeSize,
						 int			newPartSize,
						 int			minNewPartSize,
						 int			minFreeSize,
						 const std::string &	usedLabel,
						 const std::string &	freeLabel,
						 const std::string &	newPartLabel,
						 const std::string &	freeFieldLabel,
						 const std::string &	newPartFieldLabel )
{
    THROW_UNSUPPORTED( "YPartitionSplitter" );
}



bool YOptionalWidgetFactory::hasDownloadProgress()
{
    return false;
}

YDownloadProgress *
YOptionalWidgetFactory::createDownloadProgress( YWidget *		parent,
						const std::string &	label,
						const std::string & 	filename,
						YFileSize_t		expectedFileSize )
{
    THROW_UNSUPPORTED( "YDownloadProgress" );
}



bool YOptionalWidgetFactory::hasDummySpecialWidget()
{
    return true;
}

YWidget *
YOptionalWidgetFactory::createDummySpecialWidget( YWidget * parent )
{
    return YUI::widgetFactory()->createOutputField( parent, "YDummySpecialWidget" );
}

bool YOptionalWidgetFactory::hasTimezoneSelector()
{
    return false;
}

YTimezoneSelector *
YOptionalWidgetFactory::createTimezoneSelector( YWidget * parent,
                                                const std::string & _map,
                                                const std::map<std::string, std::string>& zones)
{
    THROW_UNSUPPORTED( "YTimezoneSelector" );
}


bool
YOptionalWidgetFactory::hasGraph()
{
    return false;
}


YGraph *
YOptionalWidgetFactory::createGraph( YWidget * parent, const std::string & filename,
				     const std::string & layoutAlgorithm )
{
    THROW_UNSUPPORTED( "YGraph" );
}


YGraph *
YOptionalWidgetFactory::createGraph( YWidget * parent, /* graph_t */ void * graph )
{
    THROW_UNSUPPORTED( "YGraph" );
}


bool
YOptionalWidgetFactory::hasContextMenu()
{
    return false;
}
