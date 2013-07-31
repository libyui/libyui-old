# encoding: utf-8

# Simple example for DateField
module Yast
  class DateField1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Left(DateField(Id(:date), Opt(:hstretch), "Date:", "1989-11-09")),
          Left(TimeField(Id(:time), Opt(:hstretch), "Time:", "20:15:00")),
          PushButton("&OK")
        )
      )
      UI.UserInput

      Builtins.y2milestone("Date: %1", UI.QueryWidget(Id(:date), :Value))
      Builtins.y2milestone("Time: %1", UI.QueryWidget(Id(:time), :Value))

      UI.CloseDialog

      nil
    end
  end
end

Yast::DateField1Client.new.main
