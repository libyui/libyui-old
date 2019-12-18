SET( VERSION_MAJOR "3")
SET( VERSION_MINOR "9" )
SET( VERSION_PATCH "1" )
SET( VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${GIT_SHA1_VERSION}" )

##### This is need for the libyui core, ONLY.
##### These will be overridden from exports in LibyuiConfig.cmake
# Currently you must also change so_version in libyui.spec
# *and also in **all** other* libyui-*.spec files in the other repositories.
# Yes, such a design is suboptimal.
SET( SONAME_MAJOR "11" )
SET( SONAME_MINOR "1" )
SET( SONAME_PATCH "0" )
SET( SONAME "${SONAME_MAJOR}.${SONAME_MINOR}.${SONAME_PATCH}" )
