# encoding: utf-8

# Test for UI::Beep()
module Yast
  class BeepClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(Label("Doing some operations..."))
      Builtins.sleep(4000)
      UI.CloseDialog

      UI.Beep
      UI.OpenDialog(
        VBox(
          Label("Done. Now prooceed to answer the next questions."),
          PushButton("Ok")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::BeepClient.new.main
