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
#include "YUILog.h"

#include "YUISymbols.h"
#include "YWidget.h"
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

