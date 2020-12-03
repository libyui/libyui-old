# CMakeLists.txt for libyui/examples

include( ../VERSION.cmake )

set( PKG_NAME             libyui${SONAME_MAJOR} )
set( EXAMPLES_INSTALL_DIR /usr/share/doc/packages/${PKG_NAME}/examples )


# Build a simple example named EXAMPLE from one source file EXAMPLE.cc
# and install the source to the package's doc directory
#
function( add_example EXAMPLE )
  add_executable( ${EXAMPLE} ${EXAMPLE}.cc )

  # Include header files from ../src, not /usr/include/yui
  target_include_directories( ${EXAMPLE} BEFORE PRIVATE ${PROJECT_SOURCE_DIR}/src )

  # TO DO: Use the libyui we just built, not the one from /usr/lib64
  # (It will still load the UI plug-ins from the system, not the freshly built ones)
  target_link_libraries( ${EXAMPLE} yui )
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
