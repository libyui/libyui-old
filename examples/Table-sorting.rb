# encoding: utf-8

module Yast
  class TableSortingClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Label("Library"),
          MinSize(
            30,
            10,
            Table(
              Header("Book Title", "Shelf"),
              [
                Item(Id(1), "3 Trees", " -6"),
                Item(Id(2), "missing", nil),
                Item(Id(3), "just another book", " 8a"),
                Item(Id(4), "Here comes Fred", 12),
                Item(Id(5), "Zoo", 25),
                Item(Id(6), "Lions", "balbla"),
                Item(Id(7), "Elephants ", "8b"),
                Item(Id(8), "wild animals", "a7"),
                Item(Id(9), "Weather forecast", "15yxc"),
                Item(Id(10), "my first Book", 1),
                Item(Id(11), "this is yours", 95),
                Item(Id(12), "Terra X", " 34  sdf"),
                Item(Id(13), "Programming", "dfsdf34"),
                Item(Id(14), "More programming", 0)
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

Yast::TableSortingClient.new.main
