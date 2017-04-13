SET( VERSION_MAJOR "3")
SET( VERSION_MINOR "3" )
SET( VERSION_PATCH "0" )
SET( VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${GIT_SHA1_VERSION}" )

##### This is need for the libyui core, ONLY.
##### These will be overridden from exports in LibyuiConfig.cmake
# must also adjust "Requires: yui_backend = SONAME_MAJOR" in libyui.spec.in
SET( SONAME_MAJOR "8" )
SET( SONAME_MINOR "0" )
SET( SONAME_PATCH "0" )
SET( SONAME "${SONAME_MAJOR}.${SONAME_MINOR}.${SONAME_PATCH}" )
