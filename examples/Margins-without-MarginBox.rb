# encoding: utf-8

module Yast
  class MarginsWithoutMarginBoxClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          HBox(
            HSpacing(10),
            VBox(VSpacing(2), Label("Hello, World!"), VSpacing(2)),
            HSpacing(10)
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

Yast::MarginsWithoutMarginBoxClient.new.main
