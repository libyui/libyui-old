# encoding: utf-8

# Graph1.ycp
module Yast
  class Graph1Client < Client
    def main
      Yast.import "UI"
      if !UI.HasSpecialWidget(:Graph)
        UI.OpenDialog(
          VBox(
            Label("Error: This UI doesn't support the Graph widget!"),
            PushButton(Opt(:default), "&OK")
          )
        )
        UI.UserInput
        UI.CloseDialog
        return
      end

      UI.OpenDialog(
        VBox(
          HSpacing(60),
          term(:Graph, Id(:graph), "graph1.gv", "dot"),
          PushButton(Id(:close), Opt(:default), "&Close")
        )
      )

      while true
        @widget = Convert.to_symbol(UI.UserInput)

        break if @widget == :close || @widget == :cancel
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::Graph1Client.new.main
