# encoding: utf-8

module Yast
  class BarGraph2Client < Client
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
          HSpacing(80), # force width
          HBox(
            Opt(:debugLayout),
            BarGraph(
              Opt(:vstretch),
              [600, 350, 800],
              ["Windows\nused\n%1 MB", "Windows\nfree\n%1 MB", "Linux\n%1 MB"]
            )
          ),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::BarGraph2Client.new.main
