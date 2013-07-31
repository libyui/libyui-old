# encoding: utf-8

# TimeoutUserInput.ycp
#
# Example for common usage of UI::TimeoutUserInput()
module Yast
  class TimeoutUserInputClient < Client
    def main
      Yast.import "UI"
      # Build dialog with two labels and an "OK" button.

      @countdown_sec = 30
      @interval_millisec = 200
      @countdown = Ops.divide(
        Ops.multiply(@countdown_sec, 1000),
        @interval_millisec
      )

      UI.OpenDialog(
        VBox(
          Label("Rebooting Planet Earth..."),
          Label(Id(:seconds), Builtins.sformat("%1", @countdown_sec)),
          PushButton(Id(:ok), Opt(:default), "&OK")
        )
      )

      @id = nil
      begin
        @id = UI.TimeoutUserInput(@interval_millisec)

        if @id == :timeout
          # Periodic screen update

          @countdown = Ops.subtract(@countdown, 1)
          @seconds_left = Ops.divide(
            Ops.multiply(@countdown, @interval_millisec),
            1000
          )
          UI.ChangeWidget(
            Id(:seconds),
            :Value,
            Builtins.sformat("%1", @seconds_left)
          )
        end
      end until @id == :ok || Ops.less_or_equal(@countdown, 0)

      UI.CloseDialog

      nil
    end
  end
end

Yast::TimeoutUserInputClient.new.main
