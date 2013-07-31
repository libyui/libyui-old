# encoding: utf-8

module Yast
  class MultiLineEdit2Client < Client
    def main
      Yast.import "UI"
      # Build dialog with one multi line edit field and an OK button.
      UI.OpenDialog(
        VBox(
          HSpacing(60), # force width
          HBox(
            VSpacing(7), # force height
            MultiLineEdit(
              Id(:problem),
              "Problem &description:", # label
              "No problem here"
            )
          ),
          PushButton("&OK")
        )
      )

      # Wait for user input.
      UI.UserInput

      # Get the input from the MultiLineEdit.
      #
      # Notice: The return value of UI::UserInput() does NOT return this value!
      # Rather, it returns the ID of the widget (normally the PushButton)
      # that caused UI::UserInput() to return.
      @input = Convert.to_string(UI.QueryWidget(Id(:problem), :Value))

      # Close the dialog.
      # Remember to read values from the dialog's widgets BEFORE closing it!
      UI.CloseDialog

      # Pop up a new dialog to echo the input.
      UI.OpenDialog(
        VBox(Label("You entered:"), Label(@input), PushButton("&OK"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::MultiLineEdit2Client.new.main
