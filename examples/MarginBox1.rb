# encoding: utf-8

module Yast
  class MarginBox1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          MarginBox(10, 2, Label("Hello, World!")),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::MarginBox1Client.new.main
