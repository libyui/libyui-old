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

    void addBasePath( string path );
    void addIconPath( string path );

private:

    string iconBasePath;
    list <string> icon_dirs;

    bool fileExists( string fname );
};

#endif
