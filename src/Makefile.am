#
# Makefile.am for core/libyui/src
#

AM_CXXFLAGS = -DY2LOG=\"libycp\"		\
	-fmessage-length=0

AM_CPPFLAGS = -DLOCALEDIR=\"${localedir}\"

plugin_LTLIBRARIES = libpy2UI.la

libpy2UI_la_SOURCES =				\
	YUIComponentCreator.cc			\
	YUIComponent.cc				\
	YUINamespace.cc				\
	YCP_UI.cc				\
	YCP_util.cc				\
	YCPBuiltinCaller.cc			\
						\
	YCPEvent.cc				\
	YCPValueWidgetID.cc			\
						\
	YCPDialogParser.cc			\
	YCPItemParser.cc			\
	YCPItemWriter.cc			\
	YCPMenuItemParser.cc			\
	YCPMenuItemWriter.cc			\
	YCPTableItemParser.cc			\
	YCPTableItemWriter.cc			\
	YCPTreeItemParser.cc			\
	YCPTreeItemWriter.cc			\
	YCPWizardCommandParser.cc		\
	YCPPropertyHandler.cc			\
						\
	YCPErrorDialog.cc			\
	YCPMacroPlayer.cc			\
	YCPMacroRecorder.cc



pkgincludedir = $(includedir)/ycp-ui

pkginclude_HEADERS =				\
	YUIComponentCreator.h			\
	YUIComponent.h				\
						\
	YCP_UI.h				\
	YCPDialogParser.h			\
	YCPMenuItem.h				\
	YCPValueWidgetID.h


noinst_HEADERS =				\
	YUINamespace.h				\
	YCP_util.h				\
	YCPBuiltinCaller.h			\
						\
	YCPEvent.h				\
	YCPItem.h				\
	YCPTableItem.h				\
	YCPTreeItem.h				\
						\
	YCPItemParser.h				\
	YCPItemWriter.h				\
	YCPMenuItemParser.h			\
	YCPMenuItemWriter.h			\
	YCPTableItemParser.h			\
	YCPTableItemWriter.h			\
	YCPTreeItemParser.h			\
	YCPTreeItemWriter.h			\
	YCPWizardCommandParser.h		\
	YCPPropertyHandler.h			\
						\
	YCPErrorDialog.h			\
	YCPMacroPlayer.h			\
	YCPMacroRecorder.h			\
	YCP_UI_Exception.h			\
	YWidgetOpt.h



INCLUDES =					\
	-I$(top_srcdir)/libycp/src/include	\
	-I$(top_srcdir)/liby2/src/include	\
	-I$(top_srcdir)/libyui/src/		\
	${Y2UTIL_CFLAGS}

libpy2UI_la_LDFLAGS = -version-info 2:0 -Wl,--no-undefined
libpy2UI_la_LIBADD = -lpthread \
	$(top_builddir)/libyui/src/libyui.la \
	$(top_builddir)/libycp/src/libycp.la \
	$(top_builddir)/liby2/src/liby2.la \
	$(top_builddir)/libycp/src/libycpvalues.la \
	$(top_builddir)/liby2util-r/src/liby2util.la

# "foo bar: baz" is wrong
# see "info '(automake)Multiple Outputs'"
%BuiltinCalls.h %BuiltinTable.h : Y%Namespace.h
	${top_srcdir}/base/tools/devtools/generateYCPWrappers $< $*BuiltinCalls.h $*BuiltinTable.h

BUILT_SOURCES = UIBuiltinTable.h UIBuiltinCalls.h

CLEANFILES = libyui.kdoc $(BUILT_SOURCES)


#<INSTALL-HEADER-TARGET>

# Automatically inserted by insert_install_target
# DO NOT REMOVE THE TAGS!

# Install header files only if different to version already installed -
# prevent unnecessary installations to keep make dependency changes
# and thus rebuilds for dependent packages as low as possible.

install-pkgincludeHEADERS: $(pkginclude_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(pkgincludedir)
	@list='$(pkginclude_HEADERS)';							\
	for p in $$list; do								\
		if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; 			\
		if ! cmp -s $$d$$p $(DESTDIR)$(pkgincludedir)/$$p; then			\
			echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(pkgincludedir)/$$p";	\
			$(INSTALL_DATA) $$d$$p $(DESTDIR)$(pkgincludedir)/$$p;		\
	  	fi									\
	done

#</INSTALL-HEADER-TARGET>

