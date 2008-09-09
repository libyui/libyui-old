#
# Makefile.am for libyui/src
#

# The leading . makes automake process this dir first, then the subdir

AM_CXXFLAGS =					\
	-DTHEMEDIR=\"${themedir}/current\"	\
	-DPLUGINDIR=\"${plugindir}\"		\
	-fmessage-length=0

AM_CPPFLAGS = -DLOCALEDIR=\"${localedir1}\"

lib_LTLIBRARIES = libyui.la


libyui_la_LDFLAGS = -version-info 3:0:0 -Wl,--no-undefined
libyui_la_LIBADD = -lpthread -ldl

libyui_la_SOURCES =				\
	YUI.cc					\
	YApplication.cc				\
	YWidgetFactory.cc			\
	YOptionalWidgetFactory.cc		\
						\
	YCommandLine.cc				\
	YDialogSpy.cc				\
	YEvent.cc				\
	YEventFilter.cc				\
	YEnvVar.cc				\
	YItem.cc				\
	YMacro.cc				\
	YProperty.cc				\
	YShortcut.cc				\
	YShortcutManager.cc			\
	YSimpleEventHandler.cc			\
	YSingleChildContainerWidget.cc		\
	YTableHeader.cc				\
	YTableItem.cc				\
	YTreeItem.cc				\
	YUIException.cc				\
	YUILoader.cc				\
	YUILog.cc				\
	YUIPlugin.cc				\
	YWidgetID.cc				\
						\
	YSelectionWidget.cc			\
	YSimpleInputField.cc			\
						\
	YAlignment.cc				\
	YBarGraph.cc				\
	YBusyIndicator.cc			\
	YButtonBox.cc				\
	YCheckBox.cc				\
	YCheckBoxFrame.cc			\
	YComboBox.cc				\
	YDateField.cc				\
	YDialog.cc				\
	YDownloadProgress.cc			\
	YDumbTab.cc				\
	YEmpty.cc				\
	YFrame.cc				\
	YImage.cc				\
	YInputField.cc				\
	YIntField.cc				\
	YLabel.cc				\
	YLayoutBox.cc				\
	YLogView.cc				\
	YMenuButton.cc				\
	YMultiLineEdit.cc			\
	YMultiProgressMeter.cc			\
	YMultiSelectionBox.cc			\
	YPackageSelector.cc			\
	YPartitionSplitter.cc			\
	YProgressBar.cc				\
	YPushButton.cc				\
	YRadioButton.cc				\
	YRadioButtonGroup.cc			\
	YReplacePoint.cc			\
	YRichText.cc				\
	YSelectionBox.cc			\
	YSlider.cc				\
	YSpacing.cc				\
	YSquash.cc				\
	YTable.cc				\
	YTimeField.cc				\
	YTimezoneSelector.cc			\
	YTree.cc				\
	YWidget.cc				\
	YWizard.cc				\
						\
	FSize.cc				\
	YStringTree.cc				\
	YRpmGroupsTree.cc



pkgincludedir = $(includedir)/yui

pkginclude_HEADERS =				\
	YUI.h					\
	YApplication.h				\
	YWidgetFactory.h			\
	YOptionalWidgetFactory.h		\
						\
	YBuiltinCaller.h			\
	YBothDim.h				\
	YChildrenManager.h			\
	YColor.h				\
	YCommandLine.h				\
	YDialogSpy.h				\
	YEvent.h				\
	YEventFilter.h				\
	YEnvVar.h				\
	YItem.h					\
	YMenuItem.h				\
	YMacro.h				\
	YMacroPlayer.h				\
	YMacroRecorder.h			\
	YPackageSelectorPlugin.h		\
	YProperty.h				\
	YShortcut.h				\
	YShortcutManager.h			\
	YSimpleEventHandler.h			\
	YSingleChildContainerWidget.h		\
	YTableHeader.h				\
	YTableItem.h				\
	YTreeItem.h				\
	YTypes.h				\
	YUIException.h				\
	YUILoader.h				\
	YUILog.h				\
	YUIPlugin.h				\
	YUISymbols.h				\
	YWidgetID.h				\
	YWidget_OptimizeChanges.h		\
						\
	YSelectionWidget.h			\
	YSimpleInputField.h			\
						\
	YAlignment.h				\
	YBarGraph.h				\
	YBusyIndicator.h			\
	YButtonBox.h				\
	YCheckBoxFrame.h			\
	YCheckBox.h				\
	YComboBox.h				\
	YDateField.h				\
	YDialog.h				\
	YDownloadProgress.h			\
	YDumbTab.h				\
	YEmpty.h				\
	YFrame.h				\
	YImage.h				\
	YInputField.h				\
	YIntField.h				\
	YLabel.h				\
	YLayoutBox.h				\
	YLogView.h				\
	YMenuButton.h				\
	YMultiLineEdit.h			\
	YMultiProgressMeter.h			\
	YMultiSelectionBox.h			\
	YPackageSelector.h			\
	YPartitionSplitter.h			\
	YProgressBar.h				\
	YPushButton.h				\
	YRadioButtonGroup.h			\
	YRadioButton.h				\
	YReplacePoint.h				\
	YRichText.h				\
	YSelectionBox.h				\
	YSlider.h				\
	YSpacing.h				\
	YSquash.h				\
	YTable.h				\
	YTimeField.h				\
	YTimezoneSelector.h			\
	YTree.h					\
	YWidget.h				\
	YWizard.h				\
						\
	FSize.h					\
	ImplPtr.h				\
	TreeItem.h				\
	YRpmGroupsTree.h			\
	YStringTree.h				\
	YTransText.h


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
