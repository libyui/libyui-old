# Include Directory for Libyui Plugins

This directory only consists of a symlink

    yui -> ../src

to enable consistent local builds of all libyui components (base lib and
plugins) without `make install` between changes:

Libyui plugins include libyui headers as

    #include <yui/YFoo.h>

i.e. as public includes ("<...>") from a yui/ subdirectory.

Normally, that would be /usr/include/yui. But as we develop in the libyui
project, it is common to add some changes to libyui, then to libyui-qt, then to
libyui-ncurses; all those changes should be considered one transaction to keep
everything in a consistent state.

Having this include/ subdirectory with its lone yui -> ../src symlink supports
that: In the plugins' src/CMakefile.txt, add an include path

    target_include_directories( ${MYPLUGIN} BEFORE PUBLIC ../../libyui/include )

to prefer the headers from this directory to the headers in /usr/include/yui .

This is safe even if ../../libyui/include is not available. In that case, the
system includes from /usr/include/yui are used.
