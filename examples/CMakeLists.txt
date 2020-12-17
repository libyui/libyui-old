# CMakeLists.txt for libyui/examples

include( ../VERSION.cmake )

set( PKG_NAME                   libyui${SONAME_MAJOR} )
set( EXAMPLES_INSTALL_DIR       ${DESTDIR}/usr/share/doc/packages/${PKG_NAME}/examples )

set( USE_FRESHLY_BUILT_LIBYUI   on )

if ( USE_FRESHLY_BUILT_LIBYUI )
  # Use the freshly built libyui from ../build/src, not the system libyui from
  # /usr/lib64; also set rpath.
  #
  # FIXME: This will still load the UI plug-ins from the system, not the
  # freshly built ones.
  set( LIBYUI ${CMAKE_BINARY_DIR}/src/libyui.so )
else()
  # Link against the system libyui in /usr/lib64. No rpath.
  set( LIBYUI yui )
endif()



# Build a simple example named EXAMPLE from one source file EXAMPLE.cc
# and install the source to the package's doc directory
#
function( add_example EXAMPLE )
  add_executable( ${EXAMPLE} ${EXAMPLE}.cc )
  target_link_libraries( ${EXAMPLE} ${LIBYUI} )

  if ( USE_FRESHLY_BUILT_LIBYUI )
    # Enable including headers like <yui/YFoo.h> from ../include
    # which has a symlink yui -> ../src
    target_include_directories( ${EXAMPLE} BEFORE PUBLIC ../include )
    # Add a dependency to the libyui CMake target so ../src/ is built first
    add_dependencies( ${EXAMPLE} libyui )
  endif()

  install( FILES ${EXAMPLE}.cc DESTINATION ${EXAMPLES_INSTALL_DIR} )
endfunction()

#----------------------------------------------------------------------

add_example( AutoWrap1 )
add_example( AutoWrap2 )
add_example( ComboBox1 )
add_example( ComboBox1-editable )
add_example( CustomStatusItemSelector1 )
add_example( HelloWorld )
add_example( ItemSelector1 )
add_example( ItemSelector2-minimalistic )
add_example( ManyWidgets )
add_example( MenuBar1 )
add_example( MenuBar2 )
add_example( MenuButton1 )
add_example( PollEvent )
add_example( SelectionBox1 )
add_example( SelectionBox2 )
add_example( SelectionBox3-many-items )
add_example( Table-many-items )
add_example( Table-nested-items )
