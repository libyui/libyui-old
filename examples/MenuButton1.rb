# encoding: utf-8

# Build a dialog with one menu button.
# Wait the user selects a menu entry,
# then close the dialog and terminate.
#
# Please note that it's pretty pointless to create menu entries without an ID:
# You'd never know what entry the user selected.
module Yast
  class MenuButton1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        MenuButton(
          "&Create",
          [
            Item(Id(:folder), "&Folder"),
            Item(Id(:text), "&Text File"),
            Item(Id(:image), "&Image")
          ]
        )
      )

      @id = UI.UserInput
      UI.CloseDialog

      Builtins.y2milestone("Selected: %1", @id)

      nil
    end
  end
end

Yast::MenuButton1Client.new.main
