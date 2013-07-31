# encoding: utf-8

module Yast
  class Label1DeClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Label(
            "\u00DF\u00F6\u00F6\u00F6\u00F6\u00F6\u00F6\u00DC\u00DC\u00DC\u00DC\u00DC\u00F6\u00DF\u00DF\u00DF\u00DF\u00E4\u00C4\u00C4\u00C4\u00C4"
          ),
          PushButton("\u00C4&\u00D6\u00E4")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Label1DeClient.new.main
