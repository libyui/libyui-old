# CMakeLists.txt for libyui/src

include( ../VERSION.cmake )
include( GNUInstallDirs )       # set CMAKE_INSTALL_INCLUDEDIR, ..._LIBDIR

find_package( Boost REQUIRED )


set( TARGETLIB          libyui )
set( TARGETLIB_BASE     yui    )

set( HEADERS_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/yui )


set( SOURCES
  YUI.cc
  YApplication.cc
  YWidgetFactory.cc
  YOptionalWidgetFactory.cc
  YSettings.cc
  YPath.cc
  YExternalWidgets.cc

  YCommandLine.cc
  YDialogSpy.cc
  YEvent.cc
  YEventFilter.cc
  YEnvVar.cc
  YItem.cc
  YIconLoader.cc
  YMacro.cc
  YMenuItem.cc
  YProperty.cc
  YShortcut.cc
  YShortcutManager.cc
  YSimpleEventHandler.cc
  YSingleChildContainerWidget.cc
  YTableHeader.cc
  YTableItem.cc
  YTreeItem.cc
  YUIException.cc
  YUILoader.cc
  YUILog.cc
  YUIPlugin.cc
  YWidgetID.cc

  YSelectionWidget.cc
  YSimpleInputField.cc

  YAlignment.cc
  YBarGraph.cc
  YBusyIndicator.cc
  YButtonBox.cc
  YCheckBox.cc
  YCheckBoxFrame.cc
  YComboBox.cc
  YContextMenu.cc
  YDateField.cc
  YDialog.cc
  YDialogHelpers.cc
  YDownloadProgress.cc
  YDumbTab.cc
  YEmpty.cc
  YFrame.cc
  YImage.cc
  YInputField.cc
  YIntField.cc
  YItemSelector.cc
  YLabel.cc
  YLayoutBox.cc
  YLogView.cc
  YMenuBar.cc
  YMenuButton.cc
  YMenuWidget.cc
  YMultiLineEdit.cc
  YMultiProgressMeter.cc
  YMultiSelectionBox.cc
  YPackageSelector.cc
  YGraph.cc
  YPartitionSplitter.cc
  YProgressBar.cc
  YPushButton.cc
  YRadioButton.cc
  YRadioButtonGroup.cc
  YReplacePoint.cc
  YRichText.cc
  YSelectionBox.cc
  YSlider.cc
  YSpacing.cc
  YSquash.cc
  YTable.cc
  YTimeField.cc
  YTimezoneSelector.cc
  YTree.cc
  YWidget.cc
  YWizard.cc

  FSize.cc
  YStringTree.cc
  YRpmGroupsTree.cc

  YPopupInternal.cc
  YPropertyEditor.cc
  )


set( HEADERS
  YUI.h
  YApplication.h
  YWidgetFactory.h
  YOptionalWidgetFactory.h
  YSettings.h
  YPath.h

  YBuiltinCaller.h
  YBothDim.h
  YChildrenManager.h
  YColor.h
  YCommandLine.h
  YDescribedItem.h
  YDialogSpy.h
  YEvent.h
  YEventFilter.h
  YEnvVar.h
  YItem.h
  YItemCustomStatus.h
  YIconLoader.h
  YMacro.h
  YMacroPlayer.h
  YMacroRecorder.h
  YMenuItem.h
  YPackageSelectorPlugin.h
  YGraphPlugin.h
  YProperty.h
  YShortcut.h
  YShortcutManager.h
  YSimpleEventHandler.h
  YSingleChildContainerWidget.h
  YTableHeader.h
  YTableItem.h
  YTreeItem.h
  YTypes.h
  YUIException.h
  YUILoader.h
  YUILog.h
  YUIPlugin.h
  YUISymbols.h
  YWidgetID.h
  YWidget_OptimizeChanges.h

  YSelectionWidget.h
  YSimpleInputField.h

  YAlignment.h
  YBarGraph.h
  YBusyIndicator.h
  YButtonBox.h
  YCheckBoxFrame.h
  YCheckBox.h
  YComboBox.h
  YContextMenu.h
  YDateField.h
  YDialog.h
  YDownloadProgress.h
  YDumbTab.h
  YEmpty.h
  YFrame.h
  YImage.h
  YInputField.h
  YIntField.h
  YItemSelector.h
  YLabel.h
  YLayoutBox.h
  YLogView.h
  YMenuBar.h
  YMenuButton.h
  YMenuWidget.h
  YMultiLineEdit.h
  YMultiProgressMeter.h
  YMultiSelectionBox.h
  YPackageSelector.h
  YGraph.h
  YPartitionSplitter.h
  YProgressBar.h
  YPushButton.h
  YRadioButtonGroup.h
  YRadioButton.h
  YReplacePoint.h
  YRichText.h
  YSelectionBox.h
  YSlider.h
  YSpacing.h
  YSquash.h
  YTable.h
  YTimeField.h
  YTimezoneSelector.h
  YTree.h
  YWidget.h
  YWizard.h

  FSize.h
  ImplPtr.h
  TreeItem.h
  YRpmGroupsTree.h
  YStringTree.h
  YTransText.h
  YExternalWidgetFactory.h
  YExternalWidgets.h
  )



# Add shared lib to be built
add_library( ${TARGETLIB} SHARED ${SOURCES} )

# Libraries that are needed to build this shared lib
#
# Not sure if we really need to link against Boost::boost;
# the classes used here may all be in header files only without
# any binary part that needs linking. But better safe than sorry.
target_link_libraries( ${TARGETLIB} dl pthread Boost::boost )

# https://cmake.org/cmake/help/latest/manual/cmake-properties.7.html#target-properties
set_target_properties( ${TARGETLIB} PROPERTIES
  VERSION       ${SONAME}           # From ../VERSION.cmake
  SOVERSION     ${SONAME_MAJOR}     # From ../VERSION.cmake
  OUTPUT_NAME   ${TARGETLIB_BASE}
  )


# Install the headers first so the message about the lib does not scroll away
install( FILES   ${HEADERS}   DESTINATION ${HEADERS_INSTALL_DIR} )
install( TARGETS ${TARGETLIB} LIBRARY )


# Generate Libyui_config.h where some CMake variables are expanded
# for use in the C++ code

configure_file( Libyui_config.h.in Libyui_config.h )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
