# encoding: utf-8

# Animated image example
module Yast
  class ImageScaledClient < Client
    def main
      Yast.import "UI"

      UI.OpenDialog(
        VBox(
          MinSize(
            30,
            10,
            Image(
              Opt(:scaleToFit),
              "/usr/share/wallpapers/alta-badia.jpg",
              "fallback text"
            )
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

Yast::ImageScaledClient.new.main
