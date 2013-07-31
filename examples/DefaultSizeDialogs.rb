# encoding: utf-8

# DefaultSizeDialogs.ycp
#
# Example for multiple dialogs
module Yast
  class DefaultSizeDialogsClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          Label("Top level dialog"),
          PushButton(Opt(:default), "OK\nLevel 1")
        )
      )
      UI.UserInput

      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          Label("Another dialog on top of the previous"),
          PushButton(Opt(:default), "OK\nLevel 2")
        )
      )
      UI.UserInput

      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(Label("A third dialog"), PushButton(Opt(:default), "OK\nLevel 3"))
      )
      UI.UserInput

      UI.OpenDialog(
        VBox(
          Label("An independent popup dialog"),
          PushButton(Opt(:default), "OK")
        )
      )
      UI.UserInput

      UI.OpenDialog(
        VBox(
          Label("And another independent popup dialog"),
          PushButton(Opt(:default), "OK")
        )
      )
      UI.UserInput

      UI.CloseDialog
      UI.UserInput
      UI.CloseDialog
      UI.UserInput
      UI.CloseDialog
      UI.UserInput
      UI.CloseDialog
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::DefaultSizeDialogsClient.new.main
