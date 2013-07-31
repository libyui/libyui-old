# encoding: utf-8

module Yast
  class Table5Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          MinSize(
            25,
            8,
            Table(
              Id(:table),
              Opt(:notify),
              Header("Name", "Amount"),
              [
                Item(Id(1), "Chili", 0),
                Item(Id(2), "Salami Baguette", 0),
                Item(Id(3), "Spaghetti", 0),
                Item(Id(4), "Steak Sandwich", 0)
              ]
            )
          ),
          Label("Double-click any item to increase the number"),
          Right(PushButton(Id(:cancel), "&Close"))
        )
      )

      while UI.UserInput != :cancel
        @current_item_id = Convert.to_integer(
          UI.QueryWidget(Id(:table), :CurrentItem)
        )
        @amount = Builtins.tointeger(
          Convert.to_string(UI.QueryWidget(:table, Cell(@current_item_id, 1)))
        )
        @amount = Ops.add(@amount, 1)
        Builtins.y2debug("amount: %1", @amount)
        UI.ChangeWidget(Id(:table), Cell(@current_item_id, 1), @amount)
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::Table5Client.new.main
