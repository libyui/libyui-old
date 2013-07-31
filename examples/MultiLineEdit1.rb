# encoding: utf-8

module Yast
  class MultiLineEdit1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(MultiLineEdit("Problem &description:"), PushButton("&OK"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::MultiLineEdit1Client.new.main
