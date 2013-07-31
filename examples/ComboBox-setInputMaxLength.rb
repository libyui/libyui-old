# encoding: utf-8

module Yast
  class ComboBoxSetInputMaxLengthClient < Client
    def main
      Yast.import "UI"

      UI.OpenDialog(
        VBox(
          ComboBox(
            Id(:input),
            Opt(:editable),
            "Combo Box",
            ["pizza", "pasta", "pronta"]
          ),
          IntField(Id(:field), "Limit characters to...", -1, 100, -1),
          PushButton(Id(:butt), "limit input"),
          PushButton(Id(:exitButton), "Exit")
        )
      )

      @ret = nil

      @ret = UI.UserInput

      while @ret != :exitButton
        @chars = Convert.to_integer(UI.QueryWidget(Id(:field), :Value))
        UI.ChangeWidget(:input, :InputMaxLength, @chars)
        @ret = UI.UserInput
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::ComboBoxSetInputMaxLengthClient.new.main
