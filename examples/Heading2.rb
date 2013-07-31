# encoding: utf-8

module Yast
  class Heading2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Heading("This Is a Heading."),
          Label("This is a Label."),
          PushButton("&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Heading2Client.new.main
