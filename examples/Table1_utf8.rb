# encoding: utf-8

module Yast
  class Table1Utf8Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Label("\u00D6\u00DFemsflk\u00C4\u00C4"),
          MinSize(
            25,
            7,
            Table(
              Header(
                "\u00C4nerhe\u00DF",
                "\u00C4\u00DF\u00F6\u00F6\u00E4\u00E4\u00E4\u00E4\u00E4"
              ),
              [
                Item(
                  Id(1),
                  "\u011B\u0161\u010D\u0159\u017E\u00FD\u00E1\u00ED\u02C7",
                  6
                ),
                Item(Id(2), "\u011B\u0161\u0159\u017E", nil),
                Item(Id(3), "\u00C4nder\u00DF\u00DF\u00DF", 8),
                Item(Id(4), "\u5B8C\u6642", 12),
                Item(Id(5), "Zoo", 25),
                Item(Id(6), "\u017D\u010D\u0159", 6),
                Item(Id(7), "\u00E1\u0161\u0159", 8),
                Item(Id(8), "\u00C1lklsd", 7),
                Item(Id(9), "Alphaphet", 15),
                Item(Id(10), "an", 1)
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

Yast::Table1Utf8Client.new.main
