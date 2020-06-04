SET( ${TARGETLIB}_SOURCES
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

  YPopupInternal.cc
  YPropertyEditor.cc
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
  YDescribedItem.h
  YDialogSpy.h
  YEvent.h
  YEventFilter.h
  YEnvVar.h
  YItem.h
  YItemCustomStatus.h
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
  YItemSelector.h
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
  YExternalWidgetFactory.h
  YExternalWidgets.h
)

SET( EXAMPLES_LIST
  AutoWrap1.cc
  AutoWrap2.cc
  ComboBox1.cc
  ComboBox1-editable.cc
  CustomStatusItemSelector1.cc
  HelloWorld.cc
  ItemSelector1.cc
  ItemSelector2-minimalistic.cc
  ManyWidgets.cc
  MenuButtons.cc
  PollEvent.cc
  SelectionBox1.cc
  SelectionBox2.cc
  SelectionBox3-many-items.cc
  Table-many-items.cc
)
