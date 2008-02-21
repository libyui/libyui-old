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

  File:		YDownloadProgress.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YDownloadProgress_h
#define YDownloadProgress_h

#include "YWidget.h"


class YDownloadProgressPrivate;

/**
 * DownloadProgress: A progress bar that monitors downloading a file by
 * repeatedly polling its size up to its expected size.
 **/
class YDownloadProgress : public YWidget
{
protected:
    /**
     * Constructor.
     *
     * 'label' is the label above the progress bar.
     *
     * 'filename' is the name (with path) of the file being monitored.
     *
     * 'expectedSize' is the expected size of the file in bytes.
     **/
    YDownloadProgress( YWidget *	parent,
		       const string &	label,
		       const string &	filename,
		       YFileSize_t	expectedSize );
public:
    /**
     * Destructor.
     **/
    virtual ~YDownloadProgress();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YDownloadProgress"; }

    /**
     * Get the label (the text above the progress bar).
     **/
    string label() const;

    /**
     * Set the label (the text above the progress bar).
     *
     * Derived classes are free to reimplement this, but they should call this
     * base class method at the end of the overloaded function.
     **/
    virtual void setLabel( const string & label );

    /**
     * Return the name of the file that is being monitored.
     **/
    string filename() const;

    /**
     * Set the name of a new file to monitor.
     *
     * Derived classes are free to reimplement this, but they should call this
     * base class method at the end of the overloaded function.
     **/
    virtual void setFilename( const string & filename );

    /**
     * Return the expected file size.
     **/
    YFileSize_t expectedSize() const;

    /**
     * Set the expected file size.
     *
     * Derived classes are free to reimplement this, but they should call this
     * base class method at the end of the overloaded function.
     **/
    virtual void setExpectedSize( YFileSize_t newSize );

    /**
     * Return the current size of the file that is being downloaded
     * or 0 if this file doesn't exist (yet).
     *
     * This default implementation returns the 'st_size' field of a stat()
     * system call on the file. This should be useful for most implementations.
     **/
    virtual YFileSize_t currentFileSize() const;

    /**
     * Return the percentage (0..100) of the file being downloaded so far.
     **/
    int currentPercent() const;

    /**
     * Alias for currentPercent().
     **/
    int value() const { return currentPercent(); }

    /**
     * Set a property.
     * Reimplemented from YWidget.
     *
     * This function may throw YUIPropertyExceptions.
     *
     * This function returns 'true' if the value was successfully set and
     * 'false' if that value requires special handling (not in error cases:
     * those are covered by exceptions).
     **/
    virtual bool setProperty( const string & propertyName,
			      const YPropertyValue & val );

    /**
     * Get a property.
     * Reimplemented from YWidget.
     *
     * This method may throw YUIPropertyExceptions.
     **/
    virtual YPropertyValue getProperty( const string & propertyName );

    /**
     * Return this class's property set.
     * This also initializes the property upon the first call.
     *
     * Reimplemented from YWidget.
     **/
    virtual const YPropertySet & propertySet();


private:

    ImplPtr<YDownloadProgressPrivate> priv;
};


#endif // YDownloadProgress_h
