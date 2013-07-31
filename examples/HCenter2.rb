# encoding: utf-8

module Yast
  class HCenter2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Label("This is a very long label that makes space"),
          HBox(PushButton("Normal"), HCenter(PushButton("HCenter")))
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::HCenter2Client.new.main
