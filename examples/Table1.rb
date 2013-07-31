# encoding: utf-8

module Yast
  class Table1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Heading("Today's menu"),
          MinSize(
            25,
            7,
            Table(
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
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Table1Client.new.main
