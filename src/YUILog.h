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

  File:		YUILog.h

  Author:     	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YUILog_h

#ifndef YUILogComponent
#error Missing #define YUILogComponent "myComponent" before #include "YUILog.h"
#endif

#include <iostream>

#include "ImplPtr.h"

//
// UI Logging: Macros for Application use.
//
// They all return a std::ostream &  for use with operator<<().
// #define YUILogComponent before including this header file
// to identify what subsystem ("my-ui" etc.) this log line belongs to.
//
//    #define YUILogComponent "myComponent"
//    #include <YUILog.h>
//
//    ...
//    yuiDebug() << "Creating widget" << widget << endl;
//    yuiError() << "No widget with ID " << id << endl;
//
// Unless the underlying logger function handles this differently,
// Milestone, Warning and Error are always logged, Debug only when enabled.
//

#define yuiDebug()	YUILog::debug    ( YUILogComponent, __FILE__, __LINE__, __FUNCTION__ )
#define yuiMilestone()	YUILog::milestone( YUILogComponent, __FILE__, __LINE__, __FUNCTION__ )
#define yuiWarning()	YUILog::warning  ( YUILogComponent, __FILE__, __LINE__, __FUNCTION__ )
#define yuiError()	YUILog::error    ( YUILogComponent, __FILE__, __LINE__, __FUNCTION__ )

using std::endl;
using std::hex;
using std::dec;
using std::boolalpha;
using std::noboolalpha;

//
// ------ End of user relevant part ------
//



class YUILogPrivate;

enum YUILogLevel_t
{
    YUI_LOG_DEBUG     = 0,
    YUI_LOG_MILESTONE,
    YUI_LOG_WARNING,
    YUI_LOG_ERROR
};


/**
 * Logger function.
 *
 * All const char pointer parameters might be 0.
 **/
typedef void (*YUILoggerFunction)( YUILogLevel_t,	// logLevel
				   const char *,	// logComponent
				   const char *,	// sourceFileName
				   int, 	 	// sourceLineNo
				   const char *, 	// sourceFunctionName
				   const char * );	// message

/**
 * UI logging.
 **/
class YUILog
{
public:

    /**
     * Logging functions for each log level. They all access the singleton object for this class.
     * This means that the first call to any of those functions will create the singleton YUILog object.
     **/
    static std::ostream & debug    ( const char * logComponent, const char * sourceFileName, int lineNo, const char * functionName );
    static std::ostream & milestone( const char * logComponent, const char * sourceFileName, int lineNo, const char * functionName );
    static std::ostream & warning  ( const char * logComponent, const char * sourceFileName, int lineNo, const char * functionName );
    static std::ostream & error    ( const char * logComponent, const char * sourceFileName, int lineNo, const char * functionName );

    /**
     * Generic log function.
     **/
    std::ostream & log( YUILogLevel_t	logLevel,
			const char *	logComponent,
			const char *	sourceFileName,
			int 		lineNo,
			const char * 	functionName );
    
    /**
     * Return the singleton object for this class.
     * This will create the singleton if it doesn't exist yet.
     **/
    static YUILog * instance();

    /**
     * Enable or disable debug logging.
     **/
    static void enableDebugLogging( bool debugLogging = true );

    /**
     * Return 'true' if debug logging is enabled, 'false' if not.
     **/
    static bool debugLoggingEnabled();

    /**
     * Set the UI logger function. This is the function that will ultimately
     * receive all UI log output (except debug logging if debug logging is
     * disabled).
     *
     * By default, all logging is output to stderr. This behaviour can be
     * restored if 0 is passed as a function pointer here.
     **/
    static void setLoggerFunction( YUILoggerFunction loggerFunction );

    /**
     * Return the UI logger function.
     *
     * If stderr is used for logging (i.e. no logger function set), 0 is
     * returned (unless 'returnStderrLogger' is 'true', in which case the
     * internally used stderr-logger is returned).
     **/
    static YUILoggerFunction loggerFunction( bool returnStderrLogger = false );


private:
    /**
     * Constructor.
     *
     * Not for application use - use one of the static functions above instead.
     * They all access the singleton object for this class.
     **/
    YUILog();

    /**
     * Destructor.
     **/
    ~YUILog();

    //
    // Data
    //

    ImplPtr<YUILogPrivate> priv;
};


#define YUILog_h

#endif // YUILog_h
