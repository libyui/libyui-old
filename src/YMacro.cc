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

  File:		YMacro.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include "YMacro.h"
#include "YMacroRecorder.h"
#include "YMacroPlayer.h"


YMacroRecorder * YMacro::_recorder = 0;
YMacroPlayer   * YMacro::_player   = 0;

void YMacro::setRecorder( YMacroRecorder * recorder )
{
    if ( _recorder )
	delete _recorder;

    _recorder = recorder;
}


void YMacro::setPlayer( YMacroPlayer * player )
{
    if ( _player )
	delete _player;

    _player = player;
}


void YMacro::record( const std::string & macroFile )
{
    if ( _recorder )
	_recorder->record( macroFile );
}


void YMacro::endRecording()
{
    if ( _recorder )
	_recorder->endRecording();
}


bool YMacro::recording()
{
    if ( _recorder )
	return _recorder->recording();
    else
	return false;
}


void YMacro::play( const std::string & macroFile )
{
    if ( _player )
	_player->play( macroFile );
}


void YMacro::playNextBlock()
{
    if ( _player && _player->playing() )
	_player->playNextBlock();
}


bool YMacro::playing()
{
    if ( _player )
	return _player->playing();
    else
	return false;
}


void YMacro::deleteRecorder()
{
    if ( _recorder )
	delete _recorder;
}


void YMacro::deletePlayer()
{
    if ( _player )
	delete _player;
}
