# encoding: utf-8

# Demo for shortcut checking:
# Deliberately generate a shortcut conflict.
#
# This kind of conflict cannot be resolved.
# Only one of the Widgets with "&A" will get a shortcut at all.
module Yast
  class ShortcutConflict4Client < Client
    def main
      Yast.import "UI"


      UI.OpenDialog(
        VBox(
          PushButton(Id(1), "&A"),
          PushButton(Id(2), "&A"),
          PushButton(Id(3), "&A"),
          PushButton(Id(:quit), "&Quit")
        )
      )


      @button = nil
      begin
        @button = UI.UserInput
      end while @button != :quit


      UI.CloseDialog

      nil
    end
  end
end

Yast::ShortcutConflict4Client.new.main
