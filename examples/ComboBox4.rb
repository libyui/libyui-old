# encoding: utf-8

# Create an editable combo box with restricted input character set.
module Yast
  class ComboBox4Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          ComboBox(
            Id(:addr),
            Opt(:editable),
            "Enter hex address:",
            ["0cff", "8080", "D0C0", "ffff"]
          ),
          PushButton("&OK")
        )
      )
      # Set the valid input characters.
      UI.ChangeWidget(Id(:addr), :ValidChars, "0123456789abcdefABCDEF")


      # Wait for user input.
      UI.UserInput


      # Get the input from the selection box.
      #
      # Notice: The return value of UI::UserInput() does NOT return this value!
      # Rather, it returns the ID of the widget (normally the PushButton)
      # that caused UI::UserInput() to return.
      @val = UI.QueryWidget(Id(:addr), :Value)
      Builtins.y2milestone("Selected %1", @val)

      # Close the dialog.
      # Remember to read values from the dialog's widgets BEFORE closing it!
      UI.CloseDialog


      # Pop up a new dialog to echo the input.
      UI.OpenDialog(VBox(Label("You entered:"), Label(@val), PushButton("&OK")))
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::ComboBox4Client.new.main
