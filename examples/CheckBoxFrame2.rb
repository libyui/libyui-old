# encoding: utf-8

# Example for CheckBoxFrame with inverted check box:
# The frame content becomes active if the check box is off
module Yast
  class CheckBoxFrame2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          MarginBox(
            1,
            0.5,
            CheckBoxFrame(
              Opt(:invertAutoEnable),
              "&Automatic",
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

Yast::CheckBoxFrame2Client.new.main
