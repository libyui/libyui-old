# encoding: utf-8

# Example for ButtonBox: Relaxed sanity check.
#
# Normally, a ButtonBox with more than one button is required to have one
# `okButton and one `cancelButton. With `opt(`relaxSanityCheck), this
# requirement is not enforced.
#
# Still, that option should only be used in very rare exceptions.
module Yast
  class ButtonBox3RelaxedClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          HVCenter(Label("Error: Your CPU is under water.")),
          ButtonBox(
            Opt(:relaxSanityCheck),
            PushButton(Id(:ok), "&OK"),
            PushButton(Id(:details), "&Details...")
          )
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::ButtonBox3RelaxedClient.new.main
