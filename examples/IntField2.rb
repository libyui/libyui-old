# encoding: utf-8

# Simple IntField example
module Yast
  class IntField2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          IntField(Id(:perc), "Percentage:", 0, 100, 50),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.ChangeWidget(:perc, :Value, 42)
      UI.UserInput

      @percentage = Convert.to_integer(UI.QueryWidget(:perc, :Value))
      UI.CloseDialog

      UI.OpenDialog(
        VBox(
          Label(Builtins.sformat("You entered: %1%%", @percentage)),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::IntField2Client.new.main
