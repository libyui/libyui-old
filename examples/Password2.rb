# encoding: utf-8

module Yast
  class Password2Client < Client
    def main
      Yast.import "UI"
      # Build dialog with two password fields, an "OK" and a "Cancel" button.
      UI.OpenDialog(
        VBox(
          Password(Id(:pw1), "Enter password:"),
          Password(Id(:pw2), "Confirm password:"),
          HBox(PushButton(Id(:ok), "&OK"), PushButton(Id(:cancel), "&Cancel"))
        )
      )
      @button = nil
      @pw1 = ""
      @pw2 = ""
      begin
        # Wait for Input.
        @button = UI.UserInput

        # Get the values from both password fields.
        @pw1 = Convert.to_string(UI.QueryWidget(Id(:pw1), :Value))
        @pw2 = Convert.to_string(UI.QueryWidget(Id(:pw2), :Value))

        if @button != :cancel
          if @pw1 == "" && @pw2 == ""
            # Error popup if nothing has been entered.
            UI.OpenDialog(
              VBox(Label("You must enter a password."), PushButton("&OK"))
            )
            UI.UserInput
            UI.CloseDialog
          elsif @pw1 != @pw2
            # Error popup if passwords differ.
            UI.OpenDialog(
              VBox(
                Label("The two passwords mismatch."),
                Label("Please try again."),
                PushButton("&OK")
              )
            )
            UI.UserInput
            UI.CloseDialog
          end
        end
      end until @pw1 != "" && @pw1 == @pw2 || @button == :cancel

      UI.CloseDialog

      nil
    end
  end
end

Yast::Password2Client.new.main
