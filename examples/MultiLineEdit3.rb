# encoding: utf-8

# Build dialog with MuliLineEdit widget,
# a character counter for the MuliLineEdit's contents as they are typed
# and an OK button.
module Yast
  class MultiLineEdit3Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          MultiLineEdit(
            Id(:problem),
            Opt(:notify), # make UI::UserInput() return on every keystroke
            "Problem &description:"
          ),
          HBox(
            Label("Number of characters entered:"),
            Label(Id(:char_count), "0   ")
          ),
          PushButton(Id(:ok), "&OK")
        )
      )

      @ret = nil
      begin
        # Wait for user input.
        #
        # Since the MultiLineEdit is in "notify" mode, it, too, will cause
        # UI::UserInput() to return upon any single character entered.
        @ret = UI.UserInput

        if @ret == :problem # User typed some text
          # Set the `char_count label to the number of characters entered
          # into the MultiLineEdit widget.

          UI.ChangeWidget(
            Id(:char_count),
            :Value,
            Builtins.sformat(
              "%1",
              Builtins.size(
                Convert.to_string(UI.QueryWidget(Id(:problem), :Value))
              )
            )
          )
        end
      end while @ret != :ok

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

Yast::MultiLineEdit3Client.new.main
