# encoding: utf-8

# UserInput.ycp
#
# Example for common usage of UI::UserInput()
module Yast
  class UserInputClient < Client
    def main
      Yast.import "UI"
      # Build dialog with two input fields and three buttons.
      #
      # Output goes to the log file: ~/.y2log for normal users
      # or /var/log/YaST2/y2log for root.

      @name = "Tux"
      @addr = "Antarctica"

      UI.OpenDialog(
        VBox(
          InputField(Id(:name), "&Name:", @name),
          InputField(Id(:addr), "&Address:", @addr),
          HBox(
            PushButton(Id(:ok), "&OK"),
            PushButton(Id(:cancel), "&Cancel"),
            PushButton(Id(:help), "&Help")
          )
        )
      )

      @widget_id = nil
      begin
        @widget_id = UI.UserInput

        if @widget_id == :ok
          # process "OK" button

          Builtins.y2milestone("OK button activated")


          # Retrieve widget contents

          @name = Convert.to_string(UI.QueryWidget(Id(:name), :Value))
          @addr = Convert.to_string(UI.QueryWidget(Id(:addr), :Value))
        elsif @widget_id == :cancel
          # process "Cancel" buttton
          # or window manager close button (this also returns `cancel)

          Builtins.y2milestone("Cancel button activated")
        elsif @widget_id == :help
          # process "Help" button

          Builtins.y2milestone("Help button activated")
        end 

        # No other "else" branch necessary: None of the InputField widget has
        # the `notify option set, so none of them can make UI::UserInput() return.
      end until @widget_id == :ok || @widget_id == :cancel



      # Close the dialog - but only after retrieving all information that may
      # still be stored only in its widgets: UI::QueryWidget() works only for
      # widgets that are still on the screen!

      UI.CloseDialog


      # Dump the values entered into the log file

      Builtins.y2milestone("Name: %1 Address: %2", @name, @addr)

      nil
    end
  end
end

Yast::UserInputClient.new.main
