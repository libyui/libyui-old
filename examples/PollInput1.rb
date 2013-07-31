# encoding: utf-8

module Yast
  class PollInput1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Label(Id(:lab), "Money: 800 Euro"),
          PushButton(Id(:cancel), Opt(:hstretch), "Cancel"),
          PushButton(Id(:add), Opt(:hstretch), "Add 50")
        )
      )
      @num = 800
      UI.NormalCursor

      while Ops.greater_than(@num, 0)
        @ret = UI.PollInput
        Builtins.y2milestone("%1", @ret)
        if @ret == :cancel
          break
        elsif @ret == :add
          @num = Ops.add(@num, 50)
        end
        UI.ChangeWidget(
          Id(:lab),
          :Value,
          Ops.add(Ops.add("Money: ", @num), " Euro")
        )
        Builtins.sleep(500)
        @num = Ops.subtract(@num, 1)
      end
      UI.CloseDialog

      nil
    end
  end
end

Yast::PollInput1Client.new.main
