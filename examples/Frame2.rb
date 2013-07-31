# encoding: utf-8

module Yast
  class Frame2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Frame(
            "CPU &Speed",
            RadioButtonGroup(
              VBox(
                Left(RadioButton("Normal")),
                Left(RadioButton("Overclocked")),
                Left(RadioButton("Red Hot")),
                Left(RadioButton("Melting", true))
              )
            )
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

Yast::Frame2Client.new.main
