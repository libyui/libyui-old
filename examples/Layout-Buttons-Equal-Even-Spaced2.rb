# encoding: utf-8

module Yast
  class LayoutButtonsEqualEvenSpaced2Client < Client
    def main
      Yast.import "UI"
      # Layout example:
      #
      # Build a dialog with three equal sized buttons,
      # this time with some spacing in between.
      #
      # The equal `HWeight()s will make the buttons even sized.
      # When resized larger, all buttons will retain their size.
      # Excess space will go to the HSpacing() widgets between the
      # buttons, i.e. there will be empty space between the buttons.
      #
      # Notice the importance of `opt(`hstretch) for the `HSpacing()s
      # here: This is what makes the HSpacing()s grow. Otherwise, they
      # would retain a constant size, and the buttons would grow.

      UI.OpenDialog(
        HBox(
          HWeight(1, PushButton(Opt(:default), "&OK")),
          HSpacing(Opt(:hstretch), 3),
          HWeight(1, PushButton("&Cancel everything")),
          HSpacing(Opt(:hstretch), 3),
          HWeight(1, PushButton("&Help"))
        )
      )

      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::LayoutButtonsEqualEvenSpaced2Client.new.main
