# encoding: utf-8

module Yast
  class InputField1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(VBox(InputField("Name:"), PushButton("&OK")))
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::InputField1Client.new.main
