# encoding: utf-8

module Yast
  class InputField2Client < Client
    def main
      Yast.import "UI"
      # Build dialog with one input field field and an OK button.
      UI.OpenDialog(VBox(InputField(Id(:name), "Name:"), PushButton("&OK")))

      # Wait for user input.
      UI.UserInput

      # Get the input from the input field field.
      #
      # Notice: The return value of UI::UserInput() does NOT return this value!
      # Rather, it returns the ID of the widget (normally the PushButton)
      # that caused UI::UserInput() to return.
      @name = Convert.to_string(UI.QueryWidget(Id(:name), :Value))
      Builtins.y2warning("Name: %1", @name)

      # Close the dialog.
      # Remember to read values from the dialog's widgets BEFORE closing it!
      UI.CloseDialog

      # Pop up a new dialog to echo the input.
      UI.OpenDialog(
        VBox(Label("You entered:"), Label(@name), PushButton("&OK"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::InputField2Client.new.main
