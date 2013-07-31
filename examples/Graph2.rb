# encoding: utf-8

# Graph2.ycp
module Yast
  class Graph2Client < Client
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
          HBox(
            PushButton(Id(:load1), Opt(:default), "Load 1"),
            PushButton(Id(:load2), Opt(:default), "Load 2"),
            PushButton(Id(:load3), Opt(:default), "Load 3"),
            PushButton(Id(:close), Opt(:default), "&Close")
          )
        )
      )

      while true
        @widget = Convert.to_symbol(UI.UserInput)

        case @widget
          when :load1
            UI.ChangeWidget(Id(:graph), :Filename, "graph1.gv")
          when :load2
            UI.ChangeWidget(Id(:graph), :Filename, "graph2.gv")
          when :load3
            UI.ChangeWidget(Id(:graph), :Filename, "graph3.gv")
        end

        break if @widget == :close || @widget == :cancel
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::Graph2Client.new.main
