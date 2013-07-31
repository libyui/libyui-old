# encoding: utf-8

module Yast
  class ReplacePoint1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          ReplacePoint(Id(:rp), Label("This is a label")),
          PushButton(Id(:change), "Change")
        )
      )
      UI.UserInput
      UI.ReplaceWidget(Id(:rp), PushButton("This is a PushButton"))
      UI.UserInput
      UI.ReplaceWidget(Id(:rp), CheckBox("This is a CheckBox"))
      UI.UserInput
      UI.ReplaceWidget(
        Id(:rp),
        HBox(PushButton("Button1"), PushButton("Button2"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::ReplacePoint1Client.new.main
