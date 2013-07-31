# encoding: utf-8

module Yast
  class HBox1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        HBox(PushButton("First"), PushButton("Second"), PushButton("Third"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::HBox1Client.new.main
