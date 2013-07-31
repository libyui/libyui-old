# encoding: utf-8

# Simple example for help text
module Yast
  class HelpTextClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Id(:mainLayout),
          Label("Hello, World!"),
          HBox(
            PushButton(Opt(:helpButton), "&Help"),
            PushButton(Opt(:default), "&OK")
          )
        )
      )
      UI.ChangeWidget(
        :mainLayout,
        :HelpText,
        "Oh, come on, do you really need help for this?"
      )

      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::HelpTextClient.new.main
