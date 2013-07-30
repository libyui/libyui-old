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


  File:		YCPMacroPlayer.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <stdio.h>
#include <ycp/YCPString.h>
#include <ycp/YCPVoid.h>
#include <ycp/Parser.h>
#include <ycp/YBlock.h>
#include <ycp/YCPCode.h>

#define YUILogComponent "ui-macro"
#include <yui/YUILog.h>

#include <yui/YUISymbols.h>
#include <yui/YWidget.h>
#include "YCPMacroPlayer.h"



YCPMacroPlayer::YCPMacroPlayer()
    : YMacroPlayer()
    , _macro( 0 )
    , _playing( false )
    , _error( false )
    , _nextBlockNo( -1 )
{
}


YCPMacroPlayer::~YCPMacroPlayer()
{
    // yuiDebug() << "Deleting macro player." << endl;
}


void YCPMacroPlayer::play( const string & macroFileName )
{
    _playing     = false;
    _error       = false;
    _nextBlockNo = -1;
    FILE * macroFile = fopen( macroFileName.c_str(), "r" );

    if ( ! macroFile )
    {
	_error = true;
	yuiError() << "Can't open macro file " << macroFileName << endl;
	return ;
    }

    yuiMilestone() << "Loading macro file " << macroFileName << endl;

    Parser parser( macroFile, macroFileName.c_str() );
    YCodePtr parsed = parser.parse();

    if ( ! parsed || parsed->isError() )
    {
	_error = true;
	yuiError() << "Error parsing macro file " << macroFileName
		   << " -- macro execution aborted"
		   << endl;
	return;
    }

    if ( ! parsed->isBlock() )
    {
	_error = true;
	yuiError() << "Macro syntax error in file " << macroFileName
		   << " -- expected YCP block"
		   << endl;
	return;
    }

    _macro = static_cast <YBlockPtr> (parsed) ;

    yuiMilestone() << "Playing macro from file " << macroFileName
		   << " - " << _macro->statementCount() << " macro blocks"
		   << endl;
    _nextBlockNo = 0;
    _playing = true;

    fclose( macroFile );
}


void YCPMacroPlayer::playNextBlock()
{
    if ( error() || finished() )
    {
	_playing = false;
    }

    yuiMilestone() << "Evaluating macro block #" << _nextBlockNo << endl;

    _macro->evaluate( _nextBlockNo++ );
}


bool YCPMacroPlayer::playing() const
{
    return _playing;
}


bool YCPMacroPlayer::finished()
{
    if ( error() || ! _macro || _nextBlockNo < 0 )
    {
	yuiWarning() << "Test for error() first before testing finished() !" << endl;
	return true;
    }
    yuiDebug() << "_nextBlockNo: " 	<< _nextBlockNo 
	       << ", size: "		<< _macro->statementCount()
	       << ", finished(): " 	<< boolalpha << ( _nextBlockNo >= _macro->statementCount() )
	       << endl;

    return _nextBlockNo >= _macro->statementCount();
}


void YCPMacroPlayer::rewind()
{
    _nextBlockNo = 0;
}

