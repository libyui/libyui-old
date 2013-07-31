# encoding: utf-8

# BarGraph1.ycp
module Yast
  class BarGraph1Client < Client
    def main
      Yast.import "UI"
      if !UI.HasSpecialWidget(:BarGraph)
        UI.OpenDialog(
          VBox(
            Label("Error: This UI doesn't support the BarGraph widget!"),
            PushButton(Opt(:default), "&OK")
          )
        )
        UI.UserInput
        UI.CloseDialog

        return
      end

      UI.OpenDialog(
        VBox(
          HSpacing(60), # wider default size
          BarGraph([450, 100, 700]),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::BarGraph1Client.new.main
