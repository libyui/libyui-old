# encoding: utf-8

# Animated image example
module Yast
  class ImageAnimatedClient < Client
    def main
      Yast.import "UI"

      UI.OpenDialog(
        VBox(
          MinSize(
            30,
            10,
            Image(
              Opt(:animated),
              "/usr/lib/qt3/doc/examples/widgets/trolltech.gif",
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

Yast::ImageAnimatedClient.new.main
