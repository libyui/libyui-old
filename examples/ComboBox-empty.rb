# encoding: utf-8

module Yast
  class ComboBoxEmptyClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(ComboBox("Select your Pizza:", [""]), PushButton("&OK"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::ComboBoxEmptyClient.new.main
