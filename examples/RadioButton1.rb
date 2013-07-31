# encoding: utf-8

module Yast
  class RadioButton1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        RadioButtonGroup(
          Id(:rb),
          VBox(
            Label("How do you want to crash?"),
            Left(RadioButton(Id(0), "No&w")),
            Left(RadioButton(Id(1), "&Every now and then")),
            Left(RadioButton(Id(2), "Every &five minutes", true)),
            Left(RadioButton(Id(3), Opt(:boldFont), "Ne&ver", true)),
            HBox(PushButton(Id(:next), "&Next"), PushButton("&OK"))
          )
        )
      )

      while true
        @ret = UI.UserInput
        if @ret == :next
          @current = Convert.to_integer(UI.QueryWidget(Id(:rb), :CurrentButton))
          @current = Ops.modulo(Ops.add(@current, 1), 4)
          UI.ChangeWidget(Id(:rb), :CurrentButton, @current)
        else
          break
        end
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::RadioButton1Client.new.main
