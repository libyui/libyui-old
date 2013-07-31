# encoding: utf-8

module Yast
  class Table6MultiSelClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Heading("Today's menu"),
          MinSize(
            30,
            10,
            Table(
              Id(:menu),
              Opt(:multiSelection),
              Header("Name", "Price"),
              [
                Item(Id(1), "Chili", 6),
                Item(Id(2), "Salami Baguette", nil),
                Item(Id(3), "Spaghetti", 8),
                Item(Id(4), "Steak Sandwich", 12)
              ]
            )
          ),
          PushButton("&OK")
        )
      )

      UI.ChangeWidget(:menu, :SelectedItems, [1, 2])

      UI.UserInput

      @sel = UI.QueryWidget(:menu, :SelectedItems)
      UI.CloseDialog

      Builtins.y2milestone("Selected: %1", @sel)

      nil
    end
  end
end

Yast::Table6MultiSelClient.new.main
