# CMakeLists.txt for libyui/doc

include( ../VERSION.cmake )

find_package( Doxygen REQUIRED dot )


set( PKG_NAME           libyui${SONAME_MAJOR} )
set( DOC_INSTALL_DIR    ${DOC_DESTDIR}/usr/share/doc/packages/${PKG_NAME} )

if ( DOXYGEN_FOUND )

  # Enable a HTML frame on the left side for the views tree
  set( DOXYGEN_GENERATE_TREEVIEW yes )
  # See build/CMakeDoxyfile.in for supported variables

  doxygen_add_docs( doc ../src )

else()
  message( WARNING "Missing doxygen package" )
endif()


file( GLOB DOC_FILES     *.md ) # Don't glob *.txt (CMakeLists.txt!)
file( GLOB LICENSE_FILES ../COPYING* )
file( GLOB EXAMPLE_SRC   ../examples/*.h ../examples/*.cc )

add_custom_target( install-doc
  DEPENDS doc

  COMMAND mkdir   -m 755 -p ${DOC_INSTALL_DIR}/examples
  COMMAND mkdir   -m 755 -p ${DOC_INSTALL_DIR}/html
  COMMAND install -m 644 ${DOC_FILES}     ${DOC_INSTALL_DIR}
  COMMAND install -m 644 ${LICENSE_FILES} ${DOC_INSTALL_DIR}
  COMMAND install -m 644 ${EXAMPLE_SRC}   ${DOC_INSTALL_DIR}/examples
  COMMAND cp -r ${CMAKE_CURRENT_BINARY_DIR}/html ${DOC_INSTALL_DIR}

  COMMENT "Installing documentation to ${DOC_INSTALL_DIR}" )

