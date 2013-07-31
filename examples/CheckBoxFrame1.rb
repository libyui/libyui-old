# encoding: utf-8

# Trivial example for CheckBoxFrame
module Yast
  class CheckBoxFrame1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          MarginBox(
            1,
            0.5,
            CheckBoxFrame(
              "E&xpert Settings",
              true,
              VBox(
                HBox(
                  InputField("&Server"),
                  ComboBox("&Mode", ["Automatic", "Manual", "Debug"])
                ),
                Left(CheckBox("&Logging")),
                InputField("&Connections")
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

Yast::CheckBoxFrame1Client.new.main
