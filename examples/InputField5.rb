# encoding: utf-8

module Yast
  class InputField5Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Frame(
            "Shrinkable Textentries",
            HBox(
              InputField(Opt(:shrinkable), "1"),
              InputField(Opt(:shrinkable), "2"),
              InputField(Opt(:shrinkable), "321"),
              InputField(Opt(:shrinkable), "4")
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

Yast::InputField5Client.new.main
