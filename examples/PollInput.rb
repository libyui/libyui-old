# encoding: utf-8

# PollInput.ycp
#
# Example for common usage of UI::PollInput()
module Yast
  class PollInputClient < Client
    def main
      Yast.import "UI"
      # Build dialog with two labels and a "stop" button.

      @count = 0
      @count_max = 10000

      UI.OpenDialog(
        VBox(
          Label("Calculating..."),
          Label(Id(:count), Builtins.sformat("%1 of %2", @count, @count_max)),
          PushButton(Id(:stop), "&Stop")
        )
      )

      @widget_id = nil
      begin
        @widget_id = UI.PollInput


        # Simulate heavy calculation

        Builtins.sleep(200) # milliseconds

        # Update screen to show that the program is really busy
        @count = Ops.add(@count, 1)
        UI.ChangeWidget(
          Id(:count),
          :Value,
          Builtins.sformat("%1 of %2", @count, @count_max)
        )
        UI.RecalcLayout # Might be necessary when the label becomes wider
      end until @widget_id == :stop || Ops.greater_or_equal(@count, @count_max)

      UI.CloseDialog

      nil
    end
  end
end

Yast::PollInputClient.new.main
