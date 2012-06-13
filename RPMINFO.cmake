##### RPMINFO for CMake
#####
##### THIS IS THE INFO REQUIRED FOR SPEC-FILE GENERATION

SET( SPEC_URL			"http://github.com/libyui/" )
SET( SPEC_SUMMARY		"GUI-abstraction library" )

SET( SPEC_DESCRIPTION		"This is the user interface engine that provides the abstraction from\n" )
SET( SPEC_DESCRIPTION		"${SPEC_DESCRIPTION}graphical user interfaces (Qt, Gtk) and text based user interfaces\n" )
SET( SPEC_DESCRIPTION		"${SPEC_DESCRIPTION}(ncurses).\n\n" )
SET( SPEC_DESCRIPTION		"${SPEC_DESCRIPTION}Originally developed for YaST, it can now be used independently of\n" )
SET( SPEC_DESCRIPTION		"${SPEC_DESCRIPTION}YaST for generic (C++) applications. This package has very few\n" )
SET( SPEC_DESCRIPTION		"${SPEC_DESCRIPTION}dependencies." )

SET( SPEC_BuildRequires		"boost-devel" "cmake >= 2.8" "gcc-c++" )
SET( SPEC_Provides		"libyui = %{version}" "yast2-libyui = 2.21.5" )
SET( SPEC_Obsoletes		"yast2-libyui < 2.21.5" )
SET( SPEC_DEVEL_Requires	"@PROJECTNAME@@@varpfx@_SONAME_MAJOR@ = %{version}" )
SET( SPEC_DEVEL_Requires	${SPEC_DEVEL_Requires} "glibc-devel" "libstdc++-devel" "boost-devel" )
SET( SPEC_DEVEL_Provides	"pkgconfig(@PROJECTNAME@) = %{version}" )
SET( SPEC_DEVEL_DOCS            "%doc %{_docdir}/@PROJECTNAME@" )
