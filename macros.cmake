### here go the functions and macros
### don't edit if you need something
### special. Put in a seperate file
### and include it in CMakeLists.txt

MACRO( INITIALIZE )		# compute internal library-deps and lib-type

  SET( PROGSUBDIR_UC "${PROGSUBDIR}" )
  STRING( TOLOWER "${PROGSUBDIR_UC}" PROGSUBDIR )

  SET( TARGETLIB "${BASELIB}-${PROGSUBDIR}-${PLUGINNAME}" )
  STRING( REGEX REPLACE "-+" "-" TARGETLIB "${TARGETLIB}" )
  STRING( REGEX REPLACE "-+$" "" TARGETLIB "${TARGETLIB}" )

  SET( PROJECTNAME_UC "Lib${TARGETLIB}" )
  STRING( TOLOWER "${PROJECTNAME_UC}" PROJECTNAME )

ENDMACRO( INITIALIZE )

MACRO( SET_OPTIONS )		# setup configurable options

  OPTION( DISABLE_SHARED "Shall I build a static library, only?" OFF )
  OPTION( DOCS_ONLY "Shall \"make install\" install only docs, no binaries?" OFF )
  OPTION( ENABLE_STATIC "Shall I build a static library, too?" OFF )
  OPTION( ENABLE_DEBUG "Shall I include Debug-Symbols in Release?" OFF )
  OPTION( ENABLE_EXAMPLES "Shall I compile the examples, too?" OFF )
  OPTION( ENABLE_WALL "Enable the -Wall compiler-flag?" ON )
  OPTION( ENABLE_WERROR "Enable the -Werror compiler-flag?" ON )
  OPTION( INSTALL_DOCS "Shall \"make install\" install the docs?" OFF )

ENDMACRO( SET_OPTIONS )

MACRO( SET_BUILD_FLAGS )	# setup compiler-flags depending on CMAKE_BUILD_TYPE

  SET( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined" )

  STRING( STRIP "${CMAKE_CXX_FLAGS}" CMAKE_CXX_FLAGS )
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0" )
  STRING( STRIP "${CMAKE_CXX_FLAGS}" CMAKE_CXX_FLAGS )

  IF( ENABLE_DEBUG )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3" )
  ENDIF( ENABLE_DEBUG )

  IF ( ENABLE_WALL )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )
  ENDIF ( ENABLE_WALL )

  IF ( ENABLE_WERROR )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" )
  ENDIF ( ENABLE_WERROR )

  STRING( TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_CHECK )

  FOREACH( p DEBUG RELEASE RELWITHDEBINFO MINSIZEREL )
    IF( "${p}" STREQUAL "${BUILD_TYPE_CHECK}" )
      SET( CMAKE_BUILD_TYPE ${BUILD_TYPE_CHECK} CACHE
           STRING "set to ${BUILD_TYPE_CHECK}"
           FORCE
      )
      SET( BUILD_TYPE_PASSED 1 )
    ELSE( "${p}" STREQUAL "${BUILD_TYPE_CHECK}" )
      IF( NOT BUILD_TYPE_PASSED )
        SET( BUILD_TYPE_PASSED 0 )
      ENDIF( NOT BUILD_TYPE_PASSED )
    ENDIF( "${p}" STREQUAL "${BUILD_TYPE_CHECK}" )
  ENDFOREACH()

  IF( NOT BUILD_TYPE_PASSED )
    SET( CMAKE_BUILD_TYPE RELEASE CACHE
         STRING "CMAKE_BUILD_TYPE invalid / not defined, defaulting to RELEASE"
         FORCE
    )
  ENDIF( NOT BUILD_TYPE_PASSED )

  SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g3" )
  SET( CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS} -Os" )
  SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3" )
  SET( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -O3 -g3" )

ENDMACRO( SET_BUILD_FLAGS )

MACRO( SET_ENVIRONMENT )	# setup the environment vars

  SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/resource" )

  IF( NOT DEFINED PREFIX )
    SET( PREFIX "/usr" )
  ENDIF( NOT DEFINED PREFIX )

  IF( NOT LIB_DIR )
    IF (CMAKE_SIZEOF_VOID_P MATCHES "8")
      SET( LIB_DIR "lib64" )
    ELSE (CMAKE_SIZEOF_VOID_P MATCHES "8")
      SET( LIB_DIR "lib" )
    ENDIF (CMAKE_SIZEOF_VOID_P MATCHES "8")
  ELSE( NOT LIB_DIR )
    STRING( REPLACE "${PREFIX}/" "" LIB_DIR "${LIB_DIR}" )
  ENDIF( NOT LIB_DIR )

  IF( NOT INCLUDE_DIR )
    SET( INCLUDE_DIR "include")
  ELSE( NOT INCLUDE_DIR )
    STRING( REPLACE REGEX "^${PREFIX}/" "" INCLUDE_DIR "${INCLUDE_DIR}" )
  ENDIF( NOT INCLUDE_DIR )

  SET( CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" "${PREFIX}" )
  SET( CMAKE_INSTALL_PREFIX "${PREFIX}" )
  SET( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PREFIX}/${LIB_DIR}" "${PREFIX}/${LIB_DIR}/${BASELIB}" )
  SET( CMAKE_INCLUDE_PATH "${CMAKE_INCLUDE_PATH}" "${PREFIX}/${INCLUDE_DIR}" "${EXTRA_INCLUDES}" )
  SET( INSTALL_CMAKE_DIR ${LIB_DIR}/cmake/${PROJECTNAME} CACHE PATH "Installation directory for CMake files" )
  SET( INSTALL_PKGCONFIG_DIR ${LIB_DIR}/pkgconfig CACHE PATH "Installation directory for pkgconfig files" )

  SET( LIB_DIR_PLAIN "${LIB_DIR}" )
  SET( INCLUDE_DIR "${INCLUDE_DIR}/${BASELIB}/${PROGSUBDIR_UC}/${PLUGINNAME}" )

  IF( PLUGINNAME )
    SET( LIB_DIR "${LIB_DIR}/${BASELIB}" )
  ENDIF( PLUGINNAME )

  SET( LIB_DIR "${LIB_DIR}/${PROGSUBDIR}" )

  FOREACH( p INCLUDE LIB )
      STRING( REGEX REPLACE "/+" "/" ${p}_DIR "${${p}_DIR}" )
  ENDFOREACH()

  SET( DATAROOTDIR "share" )
  SET( FULL_DATA_DIR "${DATAROOTDIR}/lib${BASELIB}" )
  SET( INSTALL_INCLUDE_DIR "${INCLUDE_DIR}" CACHE PATH "Installation directory for header files" )
  SET( INSTALL_LIB_DIR "${LIB_DIR}" CACHE PATH "Installation directory for libraries" )

  SET( THEME_DIR "${FULL_DATA_DIR}/theme" )

  IF( NOT DEFINED DOC_DIR )
    SET( DOC_DIR "${DATAROOTDIR}/doc/${PROJECTNAME}${SONAME_MAJOR}" )
  ELSE( NOT DEFINED DOC_DIR )
    STRING( REPLACE "${PREFIX}/" "" DOC_DIR "${DOC_DIR}" )
    SET( DOC_DIR "${DOC_DIR}/${PROJECTNAME}${SONAME_MAJOR}" )
  ENDIF( NOT DEFINED DOC_DIR )

  SET( INSTALL_DOC_DIR "${DOC_DIR}" )

  FOREACH( p "DOC" LIB INCLUDE CMAKE PKGCONFIG )
    SET( var "INSTALL_${p}_DIR" )
    IF( NOT IS_ABSOLUTE "${${var}}" )
      SET( ${var}_PREFIX "${PREFIX}/${${var}}" )
    ENDIF( NOT IS_ABSOLUTE "${${var}}" )
  ENDFOREACH()

ENDMACRO( SET_ENVIRONMENT )

MACRO( FIND_LIB_DEPENDENCIES )	# try to find all libs from ${LIB_DEPS}

  SET( LIB_DEPS "${LIB_DEPS}" "${INTERNAL_DEPS}" )

  FOREACH( p ${LIB_DEPS} )
    FIND_PACKAGE( ${p} REQUIRED )
    SET( CMAKE_INCLUDE_PATH "${${p}_INCLUDE_DIR}" "${CMAKE_INCLUDE_PATH}" )
    SET( CMAKE_INCLUDE_PATH "${${p}_INCLUDE_DIRS}" "${CMAKE_INCLUDE_PATH}" )
    SET( LIB_LINKER "${${p}_LIBRARIES}" "${LIB_LINKER}" )
  ENDFOREACH()

  IF( PLUGINNAME )
    FOREACH( p "" _MAJOR _MINOR _PATCH )
      SET( SONAME${p} "${Lib${BASELIB}_SONAME${p}}")
    ENDFOREACH()
  ENDIF( PLUGINNAME )

ENDMACRO( FIND_LIB_DEPENDENCIES )

MACRO( FIND_LINKER_LIBS )	# try to find all libs to be linked against

  FOREACH( p ${LIB_LINKER} )
    FIND_LIBRARY( ${p}_LOOKUP "${p}" )
    IF( "${${p}_LOOKUP}" STREQUAL "${p}_LOOKUP-NOTFOUND" )
      MESSAGE( FATAL_ERROR "Linker-Library ${p} NOT FOUND" )
    ELSE( "${${p}_LOOKUP}" STREQUAL "${p}_LOOKUP-NOTFOUND" )
      MESSAGE( STATUS "${p} found" )
    ENDIF( "${${p}_LOOKUP}" STREQUAL "${p}_LOOKUP-NOTFOUND" )
  ENDFOREACH()

ENDMACRO( FIND_LINKER_LIBS )

MACRO( CHECK_PREFIX )		# check if all internal libyui-dependecies are in prefix, too.

  FOREACH ( p ${INTERNAL_DEPS} )
    IF( NOT "${${p}_PREFIX}" STREQUAL "${PREFIX}" )
       MESSAGE( FATAL_ERROR "${p} must be installed in ${PREFIX} first! (found in ${${p}_PREFIX})" )
    ENDIF( NOT "${${p}_PREFIX}" STREQUAL "${PREFIX}" )
  ENDFOREACH ()

ENDMACRO ( CHECK_PREFIX )

MACRO( SET_AUTODOCS )		# looks for doxygen, dot and latex and setup autodocs accordingly

  FIND_PACKAGE( Doxygen )

  IF( DOXYGEN_FOUND )
    IF( DOXYGEN_DOT_FOUND )
      MESSAGE( STATUS "Found Dot: ${DOXYGEN_DOT_EXECUTABLE}" )
      SET( HAVE_DOT "YES" )
    ELSE( DOXYGEN_DOT_FOUND )
      MESSAGE( STATUS "Checking for Dot: not found" )
      SET( HAVE_DOT "NO" )
    ENDIF( DOXYGEN_DOT_FOUND )
    SET( DOXYGEN_TARGET "docs" )

    FIND_PACKAGE( LATEX )

    SET(
      LATEX_COND
      NOT ${PDFLATEX_COMPILER} STREQUAL "PDFLATEX_COMPILER-NOTFOUND"
      AND NOT ${MAKEINDEX_COMPILER} STREQUAL "MAKEINDEX_COMPILER-NOTFOUND"
    )

    IF( ${LATEX_COND} )
      MESSAGE( STATUS "Found LaTeX: ${PDFLATEX_COMPILER}" )
      MESSAGE( STATUS "Found LaTeX: ${MAKEINDEX_COMPILER}" )
      SET( BUILD_LATEX "YES" )
      STRING( RANDOM LENGTH 20 DOXYGEN_TARGET )

      ADD_CUSTOM_TARGET( docs
        ${CMAKE_BUILD_TOOL}
        DEPENDS ${DOXYGEN_TARGET}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doc/latex"
        COMMENT "Processing API documentation with LaTeX" VERBATIM
      )

      IF( INSTALL_DOCS OR DOCS_ONLY )
        INSTALL(
          FILES "${CMAKE_BINARY_DIR}/doc/latex/refman.pdf"
          DESTINATION "${INSTALL_DOC_DIR_PREFIX}"
        )
      ENDIF( INSTALL_DOCS OR DOCS_ONLY )

    ELSE( ${LATEX_COND} )
      MESSAGE( STATUS "Checking for LaTeX: not found" )
      SET( BUILD_LATEX "NO" )
    ENDIF( ${LATEX_COND} )

    ADD_CUSTOM_TARGET( ${DOXYGEN_TARGET}
      ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM
    )

    IF( INSTALL_DOCS OR DOCS_ONLY )

      FOREACH( p css gif html jpg js png tag )
        INSTALL(
          DIRECTORY "${CMAKE_BINARY_DIR}/doc/html"
          DESTINATION "${INSTALL_DOC_DIR_PREFIX}"
          FILES_MATCHING PATTERN "*.${p}"
        )
      ENDFOREACH()

    ENDIF( INSTALL_DOCS OR DOCS_ONLY )

    CONFIGURE_FILE(
      ${PROJECT_SOURCE_DIR}/resource/Doxyfile.in
      ${PROJECT_BINARY_DIR}/Doxyfile
      @ONLY
    )

    CONFIGURE_FILE(
      ${PROJECT_BINARY_DIR}/Doxyfile
      ${PROJECT_BINARY_DIR}/Doxyfile
      @ONLY
    )

  ENDIF( DOXYGEN_FOUND )

ENDMACRO( SET_AUTODOCS )

MACRO( SET_SUBDIRS )		# setup the subdirs

  FOREACH( p ${SUBDIRS} )
    ADD_SUBDIRECTORY( ${p} )
  ENDFOREACH()

ENDMACRO( SET_SUBDIRS )

MACRO( GEN_EXPORTS )		# generate and export the library-depends

  IF( ENABLE_STATIC AND NOT DISABLE_SHARED )
    EXPORT(
      TARGETS ${TARGETLIB} ${TARGETLIB}_static
      FILE "${PROJECT_BINARY_DIR}/${PROJECTNAME_UC}LibraryDepends.cmake"
    )
  ELSE( ENABLE_STATIC AND NOT DISABLE_SHARED )
    EXPORT(
      TARGETS ${TARGETLIB}
      FILE "${PROJECT_BINARY_DIR}/${PROJECTNAME_UC}LibraryDepends.cmake"
    )
  ENDIF( ENABLE_STATIC AND NOT DISABLE_SHARED )

  EXPORT( PACKAGE ${PROJECTNAME} )

ENDMACRO( GEN_EXPORTS )

MACRO( GEN_FILES )		# generate files from templates

  FOREACH( p BuildTreeSettings.cmake Config.cmake ConfigVersion.cmake )
    CONFIGURE_FILE(
      "${PROJECT_SOURCE_DIR}/resource/${p}.in"
      "${PROJECT_BINARY_DIR}/${PROJECTNAME_UC}${p}"
      @ONLY
    )
  ENDFOREACH()

  CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/resource/template.pc.in"
    "${PROJECT_BINARY_DIR}/${PROJECTNAME}.pc"
    @ONLY
  )

  IF( NOT PLUGINNAME )
    CONFIGURE_FILE(
      "${PROJECT_SOURCE_DIR}/resource/config.h.in"
      "${PROJECT_BINARY_DIR}/src/${PROJECTNAME_UC}_config.h"
      @ONLY
    )

    CONFIGURE_FILE(
      "${PROJECT_BINARY_DIR}/src/${PROJECTNAME_UC}_config.h"
      "${PROJECT_BINARY_DIR}/src/${PROJECTNAME_UC}_config.h"
      @ONLY
    )
  ENDIF( NOT PLUGINNAME )

  FOREACH( p "${PROJECTNAME_UC}Config.cmake" "${PROJECTNAME_UC}ConfigVersion.cmake" "${PROJECTNAME}.pc" )
    CONFIGURE_FILE(
      "${PROJECT_BINARY_DIR}/${p}"
      "${PROJECT_BINARY_DIR}/${p}"
      @ONLY
    )
  ENDFOREACH()

ENDMACRO( GEN_FILES )

MACRO( PREP_SPEC_FILES )

  IF( PLUGINNAME )
  SET( SPEC_Libdir "/${BASELIB}/${PROGSUBDIR_UC}" )
  STRING( REGEX REPLACE "^/+" "/" SPEC_Libdir "${SPEC_Libdir}" )
  STRING( REGEX REPLACE "/+$" "" SPEC_Libdir "${SPEC_Libdir}" )
  SET( SPEC_Owndir "%dir %{_libdir}${SPEC_Libdir}" )
  ENDIF( PLUGINNAME )

  FOREACH( p "BuildRequires" "Requires" "Provides" "Obsoletes" "Conflicts" "DEVEL_Requires" "DEVEL_Provides" )
    STRING( REPLACE "DEVEL_" "" SPEC_PREPEND "${p}" )
    FOREACH( x ${SPEC_${p}} )
      SET( ${p} "${${p}}${SPEC_PREPEND}:	${x}\n" )
    ENDFOREACH( x SPEC_${p} )
  ENDFOREACH( p "BuildRequires" "Requires" "Provides" "Obsoletes" "Conflicts" "DEVEL_Requires" "DEVEL_Provides" )

  CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/resource/template.spec.in"
    "${PROJECT_BINARY_DIR}/package/${PROJECTNAME}.spec"
    @ONLY
  )

  CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/resource/template-doc.spec.in"
    "${PROJECT_BINARY_DIR}/package/${PROJECTNAME}-doc.spec"
    @ONLY
  )

  FOREACH ( p 0 2 )
    FOREACH( x "package/${PROJECTNAME}.spec" "package/${PROJECTNAME}-doc.spec" )
      CONFIGURE_FILE(
        "${PROJECT_BINARY_DIR}/${x}"
        "${PROJECT_BINARY_DIR}/${x}"
        @ONLY
      )
    ENDFOREACH( x "package/${PROJECTNAME}.spec" "package/${PROJECTNAME}-doc.spec" )
  ENDFOREACH ( p 0 2 )

ENDMACRO( PREP_SPEC_FILES )

MACRO( PREP_OBS_TARBALL )	# prepare dist-tarball - This is a shameless rip-off from libzypp

  SET( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Package dependency solver library" )
  SET( CPACK_PACKAGE_VENDOR "SUSE" )
  SET( CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR} )
  SET( CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR} )
  SET( CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH} )
  SET( CPACK_GENERATOR "TBZ2" )
  SET( CPACK_SOURCE_GENERATOR "TBZ2" )
  SET( CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECTNAME}-${VERSION}" )
  SET( CPACK_SOURCE_IGNORE_FILES
    # temporary files
    "\\\\.swp$"
    # backup files
    "~$"
    # eclipse files
    "\\\\.cdtproject$"
    "\\\\.cproject$"
    "\\\\.project$"
    "\\\\.settings/"
    # others
    "\\\\.#"
    "/#"
    "/build/"
    "/_build/"
    "/\\\\.git/"
    # used before
    "/\\\\.libs/"
    "/\\\\.deps/"
    "\\\\.o$"
    "\\\\.lo$"
    "\\\\.la$"
    "Makefile$"
    "Makefile\\\\.in$"
    # cmake cache files
    "DartConfiguration.tcl$"
    "CMakeCache.txt"
    "CMakeFiles"
    "cmake_install.cmake$"
    "CMakeLists.txt.auto$"
    "CTestTestfile.cmake"
    "CPackConfig.cmake$"
    "CPackSourceConfig.cmake$"
    "libsolv.spec$"
  )

  INCLUDE( CPack )

  SET( TARBALL_COMMAND
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/package/*.tar.bz2
    COMMAND mkdir -p _CPack_Packages
    COMMAND ${CMAKE_MAKE_PROGRAM} package_source
    COMMAND ${CMAKE_COMMAND} -E copy ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.bz2 ${CMAKE_BINARY_DIR}/package
    COMMAND ${CMAKE_COMMAND} -E remove ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.bz2
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/ChangeLog" "${CMAKE_BINARY_DIR}/package/${PROJECTNAME}.changes"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/${PROJECTNAME}-doc.changes" "${CMAKE_BINARY_DIR}/package/${PROJECTNAME}-doc.changes"
  )

  ADD_CUSTOM_TARGET( srcpackage ${TARBALL_COMMAND} )
  ADD_CUSTOM_TARGET( srcpackage-local ${TARBALL_COMMAND} )

ENDMACRO( PREP_OBS_TARBALL )



MACRO( SUMMARY )		# prints a brief summary to stdout

  IF( NOT CMAKE_BUILD_TYPE OR NOT BUILD_TYPE_PASSED )
    MESSAGE( STATUS "" )
    MESSAGE( STATUS "********************************************************************************" )
    MESSAGE( STATUS "" )
    MESSAGE( STATUS "CMAKE_BUILD_TYPE invalid or not defined, defaulting to RELEASE" )
    MESSAGE( STATUS "Valid Values: \"DEBUG\", \"RELEASE\", \"RELWITHDEBINFO\" OR \"MINSIZEREL\"" )
  ENDIF( NOT CMAKE_BUILD_TYPE OR NOT BUILD_TYPE_PASSED )

  MESSAGE( STATUS "" )
  MESSAGE( STATUS "********************************************************************************" )
  MESSAGE( STATUS "" )
  MESSAGE( STATUS "     ${PROJECTNAME} has been configured with following options:" )
  MESSAGE( STATUS "" )
  MESSAGE( STATUS "     Plugin-Name:                           ${PLUGINNAME}" )
  MESSAGE( STATUS "     Library-Dependencies:                  ${LIB_DEPS}" )
  MESSAGE( STATUS "     Plugin is for use with:                ${PROGSUBDIR}" )
  MESSAGE( STATUS "     targetlib to build:                    ${TARGETLIB}" )
  MESSAGE( STATUS "" )
  MESSAGE( STATUS "     Version:                               ${VERSION}" )
  MESSAGE( STATUS "     SO-Version:                            ${SONAME}" )
  MESSAGE( STATUS "" )
  MESSAGE( STATUS "     Used Build-Option:                     ${CMAKE_BUILD_TYPE}" )
  MESSAGE( STATUS "     Used Compiler-Flags:                   ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}" )
  MESSAGE( STATUS "     linking against:                       ${LIB_LINKER}" )
  MESSAGE( STATUS "     using includes from:                   ${CMAKE_INCLUDE_PATH}" )
  MESSAGE( STATUS "" )

  IF( ENABLE_DEBUG OR ${CMAKE_BUILD_TYPE} STREQUAL "RELWITHDEBINFO" )
    MESSAGE( STATUS "     !!! WARNING !!!   THIS MAY RESULT IN FAT LIBS !!!" )
    MESSAGE( STATUS "                       MAKE SURE YOU REALLY NEED THIS" )
    MESSAGE( STATUS "                       (e.g. packaging or development)" )
  ENDIF( ENABLE_DEBUG OR ${CMAKE_BUILD_TYPE} STREQUAL "RELWITHDEBINFO" )

  MESSAGE( STATUS "" )
  MESSAGE( STATUS "     Install to Prefix:                     ${PREFIX}" )
  MESSAGE( STATUS "     use Shared-Dir:                        ${PREFIX}/${DATAROOTDIR}" )
  MESSAGE( STATUS "     use Library-Dir:                       ${PREFIX}/${INSTALL_LIB_DIR}" )
  MESSAGE( STATUS "     use Include-Dir:                       ${PREFIX}/${INSTALL_INCLUDE_DIR}" )
  MESSAGE( STATUS "     use CMake Plugin-InstallDir:           ${PREFIX}/${INSTALL_CMAKE_DIR}" )
  MESSAGE( STATUS "     use Doc-Dir:                           ${PREFIX}/${INSTALL_DOC_DIR}" )
  MESSAGE( STATUS "     Data-Dir:                              ${PREFIX}/${FULL_DATA_DIR}" )
  MESSAGE( STATUS "     Theme-Dir:                             ${PREFIX}/${THEME_DIR}" )
  MESSAGE( STATUS "" )
  MESSAGE( STATUS "     Disable shared library:                ${DISABLE_SHARED}" )
  IF( ENABLE_DEBUG OR ${CMAKE_BUILD_TYPE} STREQUAL "RELWITHDEBINFO" OR ${CMAKE_BUILD_TYPE} STREQUAL "DEBUG" )
    MESSAGE( STATUS "     Include Debug-Symbols in library:      ON" )
  ELSE( ENABLE_DEBUG OR ${CMAKE_BUILD_TYPE} STREQUAL "RELWITHDEBINFO" OR ${CMAKE_BUILD_TYPE} STREQUAL "DEBUG" )
    MESSAGE( STATUS "     Include Debug-Symbols in library:      OFF" )
  ENDIF( ENABLE_DEBUG OR ${CMAKE_BUILD_TYPE} STREQUAL "RELWITHDEBINFO" OR ${CMAKE_BUILD_TYPE} STREQUAL "DEBUG" )
  MESSAGE( STATUS "     Build a static library, too:           ${ENABLE_STATIC}" )
  MESSAGE( STATUS "     Build the examples, too:               ${ENABLE_EXAMPLES}" )
  MESSAGE( STATUS "" )
  MESSAGE( STATUS "********************************************************************************" )
  MESSAGE( STATUS "" )

ENDMACRO( SUMMARY )

MACRO( SET_INSTALL_TARGET )

INSTALL(
  EXPORT ${PROJECTNAME_UC}LibraryDepends
  DESTINATION "${INSTALL_CMAKE_DIR_PREFIX}"
  COMPONENT dev
)

FOREACH( p Config.cmake ConfigVersion.cmake )
  INSTALL(
    FILES "${CMAKE_BINARY_DIR}/${PROJECTNAME_UC}${p}"
    DESTINATION "${INSTALL_CMAKE_DIR_PREFIX}"
)
ENDFOREACH( p Config.cmake ConfigVersion.cmake )

INSTALL(
  FILES "${CMAKE_BINARY_DIR}/${PROJECTNAME}.pc"
  DESTINATION "${INSTALL_PKGCONFIG_DIR_PREFIX}"
)

ENDMACRO( SET_INSTALL_TARGET )
