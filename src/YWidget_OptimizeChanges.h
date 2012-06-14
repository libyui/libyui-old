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

  File:		YWidget_OptimizeChanges.h

  Author:	Michael Andres <ma@suse.de>

/-*/

#ifndef YWidget_OptimizeChanges_h
#define YWidget_OptimizeChanges_h


// Moved out of YWidget.h because it's really utterly out of place there.

#ifndef YWidget_h
#error Do not include this file from anywhere outside YWidget.h!
#endif

// class YWidget
// {

public:
    /**
     * Helper class that calls startMultipleChanges() in its constructor
     * and cares about the necessary call to doneMultipleChanges() when it goes
     * out of scope.
     **/
    class OptimizeChanges
    {
    public:
	OptimizeChanges( YWidget & w ) : yw(w)	{ yw.startMultipleChanges(); }
	~OptimizeChanges()			{ yw.doneMultipleChanges();  }
    private:
	OptimizeChanges( const OptimizeChanges & ); // no copy
	void operator=( const OptimizeChanges & );  // no assign
	YWidget & yw;
    };

// }; // class YWidget


#endif // YWidget_OptimizeChanges_h
