# encoding: utf-8

module Yast
  class HSquash1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          VCenter(
            HSquash(
              VBox(
                Left(CheckBox("short")),
                Left(CheckBox("longer")),
                Left(CheckBox("even longer")),
                Left(CheckBox("yet even longer"))
              )
            ) # Makes the VBox nonstretchable horizontally
          ), # Makes the HSquash stretchable vertically
          Left(PushButton("bottom left"))
        )
      )

      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::HSquash1Client.new.main
