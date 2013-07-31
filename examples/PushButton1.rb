# encoding: utf-8

module Yast
  class PushButton1Client < Client
    def main
      Yast.import "UI"
      # Build a dialog with one button.
      # Wait until that button is clicked,
      # then close the dialog and terminate.

      UI.OpenDialog(PushButton("&OK"))

      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::PushButton1Client.new.main
