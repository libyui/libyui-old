# encoding: utf-8

# Simple example how to use background pixmaps in alignments
module Yast
  class BackgroundPixmapClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          HVCenter(
            term(:BackgroundPixmap, "wallpapers/welcome.jpg"),
            MinSize(20, 7, SelectionBox("", ["English", "Italiano", "Klingon"]))
          ),
          Right(PushButton(Opt(:default), "&Close"))
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::BackgroundPixmapClient.new.main
