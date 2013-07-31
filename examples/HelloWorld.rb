# encoding: utf-8

module Yast
  class HelloWorldClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(Label("Hello, World!"), PushButton(Opt(:default), "&OK"))
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::HelloWorldClient.new.main
