# encoding: utf-8

# Example that scrolls a lot
module Yast
  class ScrollerClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          LogView(
            Id(:log),
            "",
            10, # visible lines
            5000
          ), # lines to store
          PushButton(Id(:cancel), "&Close")
        )
      )

      @count = 0
      @button = nil
      begin
        @count = Ops.add(@count, 1)
        @message = Builtins.sformat("[%1] Log line #%2\n", @count, @count)
        UI.ChangeWidget(:log, :LastLine, @message)
        @button = Convert.to_symbol(UI.TimeoutUserInput(200)) # millisec
      end until @button == :cancel


      UI.CloseDialog

      nil
    end
  end
end

Yast::ScrollerClient.new.main
