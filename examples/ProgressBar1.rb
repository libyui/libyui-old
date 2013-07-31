# encoding: utf-8

# Simple ProgressBar example
module Yast
  class ProgressBar1Client < Client
    def main
      Yast.import "UI"
      @max_progress = 7
      @progress = 0

      UI.OpenDialog(
        VBox(
          ProgressBar(Id(:pr), "Sample progress bar", @max_progress, @progress),
          PushButton(Id(:next), "Next"),
          Right(PushButton(Id(:close), "&Close"))
        )
      )


      while Ops.less_than(@progress, @max_progress)
        @button = Convert.to_symbol(UI.UserInput)

        if @button == :next
          @progress = Ops.add(@progress, 1)
          UI.ChangeWidget(Id(:pr), :Value, @progress)
          UI.ChangeWidget(
            Id(:pr),
            :Label,
            Builtins.sformat("Progress %1 of %2", @progress, @max_progress)
          )
        elsif @button == :close
          break
        end
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::ProgressBar1Client.new.main
