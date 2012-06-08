INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" )

SET( ${TARGETLIB}_SOURCES
  YUI.cc
  YApplication.cc
  YWidgetFactory.cc
  YOptionalWidgetFactory.cc
  YSettings.cc
  YPath.cc

  YCommandLine.cc
  YDialogSpy.cc
  YEvent.cc
  YEventFilter.cc
  YEnvVar.cc
  YItem.cc
  YIconLoader.cc
  YMacro.cc
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
  YDownloadProgress.cc
  YDumbTab.cc
  YEmpty.cc
  YFrame.cc
  YImage.cc
  YInputField.cc
  YIntField.cc
  YLabel.cc
  YLayoutBox.cc
  YLogView.cc
  YMenuButton.cc
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
)

SET( ${TARGETLIB}_HEADERS
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
  YDialogSpy.h
  YEvent.h
  YEventFilter.h
  YEnvVar.h
  YItem.h
  YIconLoader.h
  YMenuItem.h
  YMacro.h
  YMacroPlayer.h
  YMacroRecorder.h
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
  YLabel.h
  YLayoutBox.h
  YLogView.h
  YMenuButton.h
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
)

IF( DISABLE_SHARED )
  ADD_LIBRARY( ${TARGETLIB} STATIC ${${TARGETLIB}_SOURCES} )
ELSE( DISABLE_SHARED )
  ADD_LIBRARY( ${TARGETLIB} SHARED ${${TARGETLIB}_SOURCES} )
ENDIF( DISABLE_SHARED )

TARGET_LINK_LIBRARIES( ${TARGETLIB} ${LIB_LINKER} )

SET_TARGET_PROPERTIES(
  ${TARGETLIB} PROPERTIES
  SOVERSION ${SONAME_MAJOR}
  OUTPUT_NAME "${TARGETLIB}"
  PUBLIC_HEADER "${${TARGETLIB}_HEADERS}"
)

INSTALL(
  TARGETS ${TARGETLIB}
  EXPORT ${PROJECTNAME_UC}LibraryDepends
  LIBRARY DESTINATION "${INSTALL_LIB_DIR_PREFIX}"
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR_PREFIX}"
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR_PREFIX}"
  COMPONENT dev
)

INSTALL(
  FILES ${${TARGETLIB}_HEADERS}
  DESTINATION "${INSTALL_INCLUDE_DIR_PREFIX}"
)

IF( ENABLE_STATIC AND NOT DISABLE_SHARED )

  ADD_LIBRARY( ${TARGETLIB}_static STATIC ${${TARGETLIB}_SOURCES} )
  TARGET_LINK_LIBRARIES( ${TARGETLIB}_static ${LIB_LINKER} )

  SET_TARGET_PROPERTIES(
    ${TARGETLIB}_static PROPERTIES
    SOVERSION ${SONAME_MAJOR}
    OUTPUT_NAME "${TARGETLIB}"
    PUBLIC_HEADER "${${TARGETLIB}_HEADERS}"
 )

  INSTALL(
    TARGETS ${TARGETLIB}_static
    EXPORT ${PROJECTNAME_UC}LibraryDepends
    LIBRARY DESTINATION "${INSTALL_LIB_DIR_PREFIX}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR_PREFIX}"
    PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR_PREFIX}"
      COMPONENT dev
   )

ENDIF( ENABLE_STATIC AND NOT DISABLE_SHARED )

IF( INSTALL_DOCS AND DOXYGEN_FOUND )
  ADD_DEPENDENCIES( ${TARGETLIB} docs )
ENDIF( INSTALL_DOCS AND DOXYGEN_FOUND )
