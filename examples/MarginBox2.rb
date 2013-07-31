# encoding: utf-8

module Yast
  class MarginBox2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          MarginBox(
            term(:leftMargin, 10),
            term(:rightMargin, 20),
            term(:topMargin, 2),
            term(:bottomMargin, 3.5),
            Label("Hello, World!")
          ),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::MarginBox2Client.new.main
