# encoding: utf-8

module Yast
  class SelectionBox1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          SelectionBox("Select your Pizza:", ["Napoli", "Funghi", "Salami"]),
          PushButton("&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::SelectionBox1Client.new.main
