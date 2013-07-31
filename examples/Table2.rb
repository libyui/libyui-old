# encoding: utf-8

module Yast
  class Table2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Heading("Today's menu"),
          MinSize(
            30,
            7,
            Table(
              Id(:table),
              Opt(:keepSorting),
              Header("Name", Right("Price"), Center("Rating")),
              [
                Item(Id(0), "Steak Sandwich", 12, "+++"),
                Item(Id(1), "Salami Baguette", nil, "-"),
                Item(Id(2), "Chili", 6, "--"),
                Item(Id(3), "Spaghetti", 8, "+")
              ]
            )
          ),
          Right(
            HBox(
              PushButton(Id(:next), "&Next"),
              PushButton(Id(:cancel), "&Close")
            )
          )
        )
      )

      UI.ChangeWidget(Id(:table), :CurrentItem, 2)

      while UI.UserInput != :cancel
        UI.ChangeWidget(
          Id(:table),
          :CurrentItem,
          Ops.modulo(
            Ops.add(
              Convert.to_integer(UI.QueryWidget(Id(:table), :CurrentItem)),
              1
            ),
            4
          )
        )
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::Table2Client.new.main
