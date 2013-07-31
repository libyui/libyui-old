# encoding: utf-8

# Using UI::TextMode()
module Yast
  class TextModeClient < Client
    def main
      Yast.import "UI"
      @msg = UI.TextMode ? "Text mode" : "GUI mode"
      UI.OpenDialog(VBox(Label(@msg), PushButton(Opt(:default), "&OK")))
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::TextModeClient.new.main
