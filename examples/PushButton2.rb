# encoding: utf-8

module Yast
  class PushButton2Client < Client
    def main
      Yast.import "UI"
      # Build dialog with three buttons.
      # "Cancel" is the default button, i.e. pressing "Return" will
      # activate it.

      UI.OpenDialog(
        HBox(
          PushButton(Id(:ok), "&OK"),
          PushButton(Id(:cancel), Opt(:default), "&Cancel"),
          PushButton(Id(:help), "&Help")
        )
      )

      # Wait for user input. The value returned is the ID of the widget
      # that makes UI::UserInput() terminate, i.e. the respective button ID.
      @button_id = UI.UserInput

      # Close the dialog.
      UI.CloseDialog


      # Process the input.
      @button_name = ""
      if @button_id == :ok
        @button_name = "OK"
      elsif @button_id == :cancel
        @button_name = "Cancel"
      elsif @button_id == :help
        @button_name = "Help"
      end

      # Pop up a new dialog to display what button was clicked.
      UI.OpenDialog(
        VBox(
          Label(Ops.add(Ops.add("You clicked button \"", @button_name), "\".")),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::PushButton2Client.new.main
