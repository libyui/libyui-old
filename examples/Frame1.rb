# encoding: utf-8

module Yast
  class Frame1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Frame("Hey! I&mportant!", Label("Hello, World!")),
          PushButton("&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Frame1Client.new.main
