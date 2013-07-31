# encoding: utf-8

# Simple image example
module Yast
  class Image1Client < Client
    def main
      Yast.import "UI"

      UI.OpenDialog(
        VBox(
          Image(
            Id("image"),
            "/usr/share/YaST2/theme/current/wallpapers/welcome.jpg",
            "fallback text"
          ),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      if UI.WidgetExists(Id("image"))
        UI.ChangeWidget(Id("image"), :Enabled, false)
        UI.UserInput
        UI.ChangeWidget(Id("image"), :Enabled, true)
        UI.UserInput
      else
        Builtins.y2error("No such widget id")
      end
      UI.CloseDialog

      nil
    end
  end
end

Yast::Image1Client.new.main
