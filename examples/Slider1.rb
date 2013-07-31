# encoding: utf-8

module Yast
  class Slider1Client < Client
    def main
      Yast.import "UI"
      if !UI.HasSpecialWidget(:Slider)
        UI.OpenDialog(
          VBox(
            Label("Error: This UI doesn't support the Slider widget!"),
            PushButton(Opt(:default), "&OK")
          )
        )
        UI.UserInput
        UI.CloseDialog

        return
      end

      UI.OpenDialog(
        VBox(Slider("Percentage", 0, 100, 50), PushButton(Opt(:default), "&OK"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Slider1Client.new.main
