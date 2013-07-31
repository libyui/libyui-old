# encoding: utf-8

module Yast
  class HStretch1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Label("Some text goes here"),
          Label("This is some more text, that is quite long, as you can see."),
          HBox(PushButton("&OK"), HStretch())
        )
      )
      @ret = UI.UserInput
      UI.CloseDialog
      deep_copy(@ret)
    end
  end
end

Yast::HStretch1Client.new.main
