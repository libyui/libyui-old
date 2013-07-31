# encoding: utf-8

# Simple BusyIndicator example
module Yast
  class BusyIndicatorClient < Client
    def main
      Yast.import "UI"
      @timeout = 3000 # in milisenconds

      UI.OpenDialog(
        VBox(
          BusyIndicator(Id(:busy), "Sample busy indicator", @timeout),
          PushButton(Id(:alive), "send &tick"),
          Right(PushButton(Id(:close), "&Close"))
        )
      )


      while true
        @button = Convert.to_symbol(UI.TimeoutUserInput(100))

        if @button == :alive
          UI.ChangeWidget(Id(:busy), :Alive, true)
        elsif @button == :close
          break
        end
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::BusyIndicatorClient.new.main
