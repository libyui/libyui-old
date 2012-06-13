##### PROJECTINFO for CMake

SET( BASELIB		"yui" )		# don't change this

##### MAKE ALL NEEDED CHANGES HERE #####

SET( SUBDIRS		src examples )
SET( LIB_DEPS		Boost )
SET( LIB_LINKER		dl pthread )
