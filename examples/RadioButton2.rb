# encoding: utf-8

module Yast
  class RadioButton2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        RadioButtonGroup(
          Id(:rb),
          VBox(
            Label("How do you want to crash?"),
            Left(RadioButton(Id(0), Opt(:notify), "No&w")),
            Left(RadioButton(Id(1), Opt(:notify), "&Every now an then")),
            Left(RadioButton(Id(2), Opt(:notify), "Every &five minutes")),
            Left(RadioButton(Id(3), Opt(:notify), "Ne&ver", true)),
            HBox(
              PushButton(Id(:next), "&Next"),
              PushButton(Id(:close), "&Close")
            )
          )
        )
      )

      while true
        @ret = UI.UserInput

        if @ret == :close
          break
        elsif @ret == :next
          # y2milestone("Hit next");
          @current = Convert.to_integer(UI.QueryWidget(Id(:rb), :CurrentButton))
          @current = Ops.modulo(Ops.add(@current, 1), 4)
          UI.ChangeWidget(Id(:rb), :CurrentButton, @current)
        else
          Builtins.y2milestone("Hit RadioButton #%1", @ret)
        end
      end

      Builtins.y2milestone("Terminating.")

      UI.CloseDialog

      nil
    end
  end
end

Yast::RadioButton2Client.new.main
