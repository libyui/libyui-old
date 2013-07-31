# encoding: utf-8

module Yast
  class ShortcutConflict6Client < Client
    def main
      Yast.import "UI"
      # Demo for shortcut checking:
      # Only exotic characters, not enough valid shortcut characters.

      UI.OpenDialog(
        HBox(
          PushButton("&\u00A7\u00A7"),
          PushButton("&???"),
          PushButton("?&??"),
          PushButton("&!!!"),
          PushButton("&***"),
          PushButton("&OK")
        )
      )

      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::ShortcutConflict6Client.new.main
