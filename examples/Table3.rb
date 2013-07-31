# encoding: utf-8

# Table example: Exchange complete table content
module Yast
  class Table3Client < Client
    def main
      Yast.import "UI"
      @foodItems = [
        Item(Id(3), "Spaghetti", 8),
        Item(Id(4), "Steak Sandwich", 12),
        Item(Id(1), "Chili", 6),
        Item(Id(2), "Salami Baguette", nil)
      ]

      @carItems = [
        Item(Id(0), "Mercedes", 60000),
        Item(Id(1), "Audi", 50000),
        Item(Id(2), "VW", 40000),
        Item(Id(3), "BMW", 60000),
        Item(Id(3), "Porsche", 80000)
      ]

      @itemLists = [@foodItems, @carItems]

      @listNo = 0

      UI.OpenDialog(
        VBox(
          Heading("Prices"),
          MinSize(
            30,
            10,
            Table(Id(:table), Header("Name", "Price"), @foodItems)
          ),
          Right(
            HBox(
              PushButton(Id(:next), "Change &Table Contents"),
              PushButton(Id(:cancel), "&Close")
            )
          )
        )
      )

      while UI.UserInput != :cancel
        # Change table contents

        @listNo = Ops.subtract(1, @listNo)
        UI.ChangeWidget(:table, :Items, Ops.get(@itemLists, @listNo))

        # Double check: Retrieve contents and dump to log
        Builtins.y2milestone(
          "New table content:\n%1",
          UI.QueryWidget(:table, :Items)
        )
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::Table3Client.new.main
