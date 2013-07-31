# encoding: utf-8

module Yast
  class ShortcutConflict1Client < Client
    def main
      Yast.import "UI"
      # Demo for shortcut checking:
      # Deliberately generate a shortcut conflict.

      UI.OpenDialog(
        HBox(
          PushButton("&OK"),
          PushButton("Also &OK"),
          PushButton("Do Something"),
          PushButton("Quick && Dirty"),
          PushButton("&Cancel")
        )
      )

      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::ShortcutConflict1Client.new.main
