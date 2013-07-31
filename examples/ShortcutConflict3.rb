# encoding: utf-8

module Yast
  class ShortcutConflict3Client < Client
    def main
      Yast.import "UI"
      # Demo for shortcut checking:
      # Deliberately generate a shortcut conflict.

      UI.OpenDialog(
        VBox(
          RadioButtonGroup(
            VBox(
              Left(RadioButton("Instant &Play", true)),
              Left(RadioButton("Demo &Only"))
            )
          ),
          Left(
            ComboBox("Game &Type:", ["Boring", "Full automatic", "Unplayable"])
          ),
          InputField("Nickname for &Player 1:"),
          InputField("Nickname for &Player 2:"),
          IntField("Maximum &Time:", 0, 100, 20),
          PushButton("&OK")
        )
      )

      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::ShortcutConflict3Client.new.main
