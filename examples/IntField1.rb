# encoding: utf-8

module Yast
  class IntField1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          IntField("Percentage:", 0, 100, 50),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::IntField1Client.new.main
