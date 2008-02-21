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
