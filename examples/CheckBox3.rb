# encoding: utf-8

module Yast
  class CheckBox3Client < Client
    def main
      Yast.import "UI"
      # Build dialog with one check box and buttons to set its state to
      # on, off or "don't care" (tri-state).

      UI.OpenDialog(
        VBox(
          CheckBox(Id(:cb), "Format hard disk"),
          HBox(
            HWeight(1, PushButton(Id(:setOn), "Set on")),
            HWeight(1, PushButton(Id(:setOff), "Set off")),
            HWeight(1, PushButton(Id(:dontCare), "Don't care"))
          ),
          PushButton(Id(:ok), "&OK")
        )
      )


      # Input loop. Will be left only after 'OK' is clicked.

      @button = nil
      begin
        @button = UI.UserInput

        if @button == :setOn
          UI.ChangeWidget(Id(:cb), :Value, true)
        elsif @button == :setOff
          UI.ChangeWidget(Id(:cb), :Value, false)
        elsif @button == :dontCare
          UI.ChangeWidget(Id(:cb), :Value, nil)
        end
      end until @button == :ok


      # Get the check box's value.
      #
      # Notice: The return value of UI::UserInput() does NOT return this value!
      # Rather, it returns the ID of the widget (normally the PushButton)
      # that caused UI::UserInput() to return.

      @cb_val = Convert.to_boolean(UI.QueryWidget(Id(:cb), :Value))

      # Close the dialog.
      # Remember to read values from the dialog's widgets BEFORE closing it!
      UI.CloseDialog

      # Convert the check box value to string.
      @valStr = "Don't care"
      @valStr = "Yes" if @cb_val == true
      @valStr = "No" if @cb_val == false

      # Pop up a new dialog to echo the input.
      UI.OpenDialog(
        VBox(Label("Your selection:"), Label(@valStr), PushButton("&OK"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::CheckBox3Client.new.main
