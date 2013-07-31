# encoding: utf-8

# Example for ButtonBox
module Yast
  class ButtonBox1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          HVCenter(Label("Hello, world!")),
          ButtonBox(
            PushButton(Id(:doit1), "Do &Something Very Cool"),
            PushButton(Id(:doit2), Opt(:key_F10, :customButton), "Do &More"),
            PushButton(Id(:help), "&Help"),
            PushButton(Id(:ok), "&OK"),
            PushButton(Id(:cancel), "&Cancel"),
            PushButton(Id(:apply), "&Apply")
          )
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::ButtonBox1Client.new.main
