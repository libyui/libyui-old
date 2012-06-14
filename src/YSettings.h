/*
 |****************************************************************************
 |
 | Copyright (c) 2012 Björn Esser
 | All Rights Reserved.
 |
 | This file is part of libYUI which is
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

  File:		YSettings.h

  Author:	Björn Esser <bjoern.esser@gmail.com>

/-*/

#ifndef YSettings_h
#define YSettings_h

#include <string>

/**
 * Settings for libyui
 *
 * This singleton-object hold some presets for libyui.
 **/
class YSettings
{
public:
    /**
     * This returns a pointer to to the YSettings singleton object.
     *
     * If there is no object, when called, the object gets initialised.
     **/
    static YSettings * access ();
    /**
     * This can be used to set a subdir beneath PLUGINDIR or THEMEDIR,
     * where your program stores a custom plugin or theme.
     *
     * Once this is set, it can't be altered. If you do so although an
     * exception will be thrown.
     **/
    static void setProgSubDir ( std::string directory );
    /**
     * Returns the value of your program's subdir.
     **/
    static std::string getProgSubDir ();

private:
    static YSettings * _instance;
    static std::string progSubDir;
    YSettings ();
    YSettings ( const YSettings& );
    ~YSettings ();
};

#endif // YSettings_h
