# encoding: utf-8

module Yast
  class Heading1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(VBox(Heading("This Is a Heading."), PushButton("&OK")))
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Heading1Client.new.main
