#ifndef YIconLoader_H
#define YIconLoader_H

#include <string>
#include <list>


using namespace std;

class YIconLoader 
{
public:

    YIconLoader();
    ~YIconLoader();

    string findIcon( string name );

    //FIXME: these two are here for compatibility reasons
    // deprecate them in due course and treat base path just 
    // like any other search path
    void setIconBasePath( string path );
    string iconBasePath() const;

    void addIconSearchPath( string path );

private:

    string _iconBasePath;
    list <string> icon_dirs;

    bool fileExists( string fname );
};

#endif
