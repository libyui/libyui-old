# encoding: utf-8

module Yast
  class MultiLineEditSetInputMaxLengthClient < Client
    def main
      Yast.import "UI"

      UI.OpenDialog(
        VBox(
          MultiLineEdit(Id(:input), "Multi Line Edit", "pizza\npasta\npronta"),
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

Yast::MultiLineEditSetInputMaxLengthClient.new.main
