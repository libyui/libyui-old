# encoding: utf-8

module Yast
  class Label1JaClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(VBox(Label("\u5B8C\u6642"), PushButton("&OK\u5B8C\u6642")))
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Label1JaClient.new.main
