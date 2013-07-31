# encoding: utf-8

module Yast
  class VBox1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(PushButton("First"), PushButton("Second"), PushButton("Third"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::VBox1Client.new.main
