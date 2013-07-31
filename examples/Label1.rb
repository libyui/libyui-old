# encoding: utf-8

module Yast
  class Label1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(VBox(Label("Hello, World!"), PushButton("&OK")))
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Label1Client.new.main
