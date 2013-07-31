# encoding: utf-8

# Simple example for TimeField
module Yast
  class TimeField1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(TimeField(Id(:time), "Time:", "13:15:00"), PushButton("&OK"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::TimeField1Client.new.main
