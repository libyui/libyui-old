# encoding: utf-8

module Yast
  class SmallDecorationsClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:smallDecorations),
        VBox(Label("Hello, World!"), PushButton(Opt(:default), "&OK"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::SmallDecorationsClient.new.main
