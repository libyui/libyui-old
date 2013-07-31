# encoding: utf-8

module Yast
  class Table7MultiSelClient < Client
    def main
      Yast.import "UI"
      @new_items = [
        Item(Id(1), "Mercedes", 60000),
        Item(Id(2), "Audi", 50000),
        Item(Id(3), "VW", 40000),
        Item(Id(4), "BMW", 60000),
        Item(Id(5), "Porsche", 80000)
      ]

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
          HBox(
            PushButton(Id(:next), "Change &Table Contents"),
            PushButton(Id(:cancel), "&OK")
          )
        )
      )

      UI.ChangeWidget(:menu, :SelectedItems, [1, 2])


      while UI.UserInput != :cancel
        # Change table contents
        UI.ChangeWidget(:menu, :Items, @new_items)

        # Double check: Retrieve contents and dump to log
        Builtins.y2milestone(
          "New table content:\n%1",
          UI.QueryWidget(:menu, :Items)
        )
      end
      @sel = UI.QueryWidget(:menu, :SelectedItems)
      UI.CloseDialog

      Builtins.y2milestone("Selected: %1", @sel)

      nil
    end
  end
end

Yast::Table7MultiSelClient.new.main
