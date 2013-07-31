# encoding: utf-8

module Yast
  class SelectionBox5Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          SelectionBox(
            Opt(:shrinkable),
            "Minimalistic selbox (rather ugly): ",
            [
              "Napoli",
              "Funghi",
              "Salami",
              "Quattro Stagioni",
              "Caprese",
              "Mista"
            ]
          ),
          PushButton("&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::SelectionBox5Client.new.main
