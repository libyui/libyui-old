# encoding: utf-8

module Yast
  class Weight1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        HBox(
          HWeight(1, PushButton("First Button (W: 50)")),
          PushButton("Small Button"),
          HWeight(
            1,
            PushButton(
              "Second Button (Weight 50 - this one determines the total width"
            )
          )
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Weight1Client.new.main
