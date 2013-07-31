# encoding: utf-8

module Yast
  class LogView2Client < Client
    def main
      Yast.import "UI"

      @button = nil


      UI.OpenDialog(
        VBox(
          LogView(
            Id(:log),
            "Log View ",
            15, # visible lines
            0
          ), # lines to store
          PushButton(Id(:ok), "&OK")
        )
      )
      begin
        @line = Builtins.tostring(Builtins.random(100))
        @line = Ops.add(@line, "\n")
        UI.ChangeWidget(Id(:log), :LastLine, @line)
        @button = UI.TimeoutUserInput(100)
      end until @button == :ok

      UI.CloseDialog

      nil
    end
  end
end

Yast::LogView2Client.new.main
