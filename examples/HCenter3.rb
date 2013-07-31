# encoding: utf-8

module Yast
  class HCenter3Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          VCenter(PushButton(Opt(:vstretch), "Button 1")),
          VCenter(PushButton(Opt(:vstretch), "Button 2")),
          VCenter(PushButton(Opt(:vstretch), "Button 3"))
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::HCenter3Client.new.main
