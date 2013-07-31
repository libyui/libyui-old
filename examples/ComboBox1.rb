# encoding: utf-8

module Yast
  class ComboBox1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          ComboBox("Select your Pizza:", ["Napoli", "Funghi", "Salami"]),
          PushButton("&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::ComboBox1Client.new.main
