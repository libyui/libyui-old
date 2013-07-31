# encoding: utf-8

# Label example using bold font
module Yast
  class Label4Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Label("Label using normal font"),
          Label(Opt(:boldFont), "Label using bold font"),
          Label("Label using normal font"),
          PushButton(Opt(:default), "&OK")
        )
      )

      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Label4Client.new.main
