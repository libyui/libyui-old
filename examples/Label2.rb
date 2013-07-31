# encoding: utf-8

module Yast
  class Label2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(Label("Labels can have\nmultiple lines."), PushButton("&OK"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Label2Client.new.main
