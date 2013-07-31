# encoding: utf-8

module Yast
  class Label1CsClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Label("\u011B\u0161\u010D\u0159\u017E\u00FD\u00E1\u00ED\u02C7"),
          PushButton("&OK\u011B\u0161")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Label1CsClient.new.main
