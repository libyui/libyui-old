# encoding: utf-8

module Yast
  class LayoutButtonsEqualGrowingClient < Client
    def main
      Yast.import "UI"
      # Layout example:
      #
      # Build a dialog with three equal sized buttons.
      #
      # The equal `HWeight()s will make the buttons equal sized.
      # When resized, all buttons will resize equally in order to
      # maintain the equal layout weights.

      UI.OpenDialog(
        HBox(
          HWeight(1, PushButton(Opt(:default), "&OK")),
          HWeight(1, PushButton("&Cancel everything")),
          HWeight(1, PushButton("&Help"))
        )
      )

      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::LayoutButtonsEqualGrowingClient.new.main
