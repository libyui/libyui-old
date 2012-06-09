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
