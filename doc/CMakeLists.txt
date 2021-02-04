# CMakeLists.txt for libyui/doc

include( ../VERSION.cmake )

find_package( Doxygen REQUIRED dot )


set( PKG_NAME           libyui${SONAME_MAJOR} )
set( DOC_INSTALL_DIR    ${DOC_DESTDIR}/usr/share/doc/packages/${PKG_NAME} )


#
# Doxygen-generated autodocs
#

if ( DOXYGEN_FOUND )

  set( DOXYGEN_GENERATE_TREEVIEW yes ) # Enable views tree HTML frame
  set( DOXYGEN_QUIET             yes ) # Less verbose output
  set( DOXYGEN_WARN_LOGFILE      doxygen-warnings.log )
  # See build/CMakeDoxyfile.in for more supported variables

  doxygen_add_docs( doc ../src )

else()
  message( WARNING "Missing doxygen package" )
endif()


#
# Other (manually written) docs
#

file( GLOB DOC_FILES *.md ) # Collect all *.md files by wildcard
# Don't glob *.txt (CMakeLists.txt!)
# The license files (../COPYING*) are handled in the .spec file



# Define a new custom make target "install-doc".
# Notice that we can't use DESTDIR here since CMake only sets that
# for "make install", not for this custom "make install-doc".
# https://gist.github.com/shundhammer/12b6343309544a5920f51cd0063eeecf

add_custom_target( install-doc
  DEPENDS doc

  COMMAND mkdir   -m 755 -p ${DOC_INSTALL_DIR}/html
  COMMAND install -m 644 ${DOC_FILES} ${DOC_INSTALL_DIR}
  COMMAND cp -r ${CMAKE_CURRENT_BINARY_DIR}/html ${DOC_INSTALL_DIR}

  COMMENT "Installing documentation to ${DOC_INSTALL_DIR}" )

