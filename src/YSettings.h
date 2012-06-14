/*
  Copyright (c) 2012 Björn Esser 

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
  SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
  THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
