# encoding: utf-8

module Yast
  class HCenter1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Label("This is a long label which makes space"),
          HBox(Label("A"), HCenter(Label("B")), Label("C"))
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::HCenter1Client.new.main
