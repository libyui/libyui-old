# encoding: utf-8

module Yast
  class Password1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(VBox(Password("Enter password:"), PushButton("&OK")))
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Password1Client.new.main
