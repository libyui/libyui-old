#define YUILogComponent "ui"
#include "YUILog.h"

#include <sys/stat.h>
#include <sstream>

#include "YIconLoader.h"

#define FALLBACK_ICON_PATH "/usr/share/icons/hicolor/"

YIconLoader::YIconLoader()
{
    addIconSearchPath(FALLBACK_ICON_PATH);
}

YIconLoader::~YIconLoader()
{
}

void YIconLoader::setIconBasePath( string path)
{
    _iconBasePath = path;
}

string YIconLoader::iconBasePath() const
{
    return _iconBasePath;
}

void YIconLoader::addIconSearchPath( string path )
{
    icon_dirs.push_front( path );
}

string YIconLoader::findIcon( string name )
{
    // No extension -> add some 
    string::size_type loc = name.find(".png");
    if ( loc == string::npos )
	name += ".png";

    // Absolute path -> return it
    if (name[0] == '/')
        return name;

    string fullPath;

    // Look in global search path
    if ( !_iconBasePath.empty () )
    {
	fullPath = _iconBasePath + name;
        if ( fileExists ( fullPath ) )
	{
	    yuiMilestone() << "Found " << name << " in global search path" << endl;
	    return fullPath;
	}
    }

    // Now search the fallback dirs
    list<string>::iterator listIt = icon_dirs.begin();

    while( listIt != icon_dirs.end() )
    {
	// Something like relative path
	if ( name.find('/') != string::npos ) 
            fullPath = *listIt +  name;
	// No '/' chars, just the name -> use '22x22/apps' fallback
	else 
	   fullPath = *listIt + "22x22/apps/" + name;

        if ( fileExists( fullPath ) )
	{	
	    yuiMilestone() << "Found " << name << " in " <<  *listIt << " search path" << endl;
	    return fullPath;
	}

	yuiMilestone() <<  name << " not found in " <<  *listIt << " search path, skipping" << endl;
        listIt++;
    }

    return "";
}

bool YIconLoader::fileExists( string fname )
{
    struct stat fileInfo;
    int ret = stat (fname.c_str(), &fileInfo);

    return ( ret == 0 );
}
