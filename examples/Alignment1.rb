# encoding: utf-8

module Yast
  class Alignment1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          PushButton(
            "This is a very long button - it reserves extra space for the label."
          ),
          HBox(
            PushButton(Opt(:hstretch), "Stretchable button"),
            ReplacePoint(Id(:rp), Label("Label"))
          )
        )
      )
      UI.UserInput

      UI.ReplaceWidget(Id(:rp), Left(Label("Left")))
      UI.UserInput

      UI.ReplaceWidget(Id(:rp), Right(Label("Right")))
      UI.UserInput

      UI.ReplaceWidget(Id(:rp), HCenter(Label("HCenter")))
      UI.UserInput

      nil
    end
  end
end

Yast::Alignment1Client.new.main
