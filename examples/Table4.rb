# encoding: utf-8

module Yast
  class Table4Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Heading("Today's menu"),
          MinSize(
            25,
            7,
            Table(
              Id(:table),
              Header("Name", "Price"),
              [
                Item(Id(1), "Chili", 6),
                Item(Id(2), "Salami Baguette", nil),
                Item(Id(3), "Spaghetti", 8),
                Item(Id(4), "Steak Sandwich", 12)
              ]
            )
          ),
          Right(HBox(PushButton("&Lookup"), PushButton(Id(:cancel), "&Close")))
        )
      )

      while UI.UserInput != :cancel
        @id = UI.QueryWidget(Id(:table), :CurrentItem)
        if Ops.is_integer?(@id)
          @text = Builtins.sformat(
            "Line: %1",
            UI.QueryWidget(Id(:table), term(:Item, @id))
          )
          UI.OpenDialog(
            MarginBox(
              1,
              0.2,
              VBox(
                Left(Label("Current Table Item")),
                Label(Opt(:outputField), @text),
                PushButton("&OK")
              )
            )
          )
          UI.UserInput
          UI.CloseDialog
        end
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::Table4Client.new.main
