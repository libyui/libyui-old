# encoding: utf-8

# Simple example for MinWidth widget
module Yast
  class MinWidth1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          # SelectionBox blown up with MinWidth
          MinWidth(40, SelectionBox("", ["Napoli", "Funghi", "Salami"])),
          # All hstretchable widgets in the same VBox will get
          # at least as wide as specified with MinWidth
          SelectionBox("", ["Napoli", "Funghi", "Salami"]),
          # The same SelectionBox with default width
          # `Left is necessary to take away horizontal stretchability
          Left(SelectionBox("", ["Napoli", "Funghi", "Salami"])),
          PushButton("&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::MinWidth1Client.new.main
