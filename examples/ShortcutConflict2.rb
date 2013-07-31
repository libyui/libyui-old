# encoding: utf-8

module Yast
  class ShortcutConflict2Client < Client
    def main
      Yast.import "UI"
      # Demo for shortcut checking:
      # Deliberately generate a shortcut conflict.

      UI.OpenDialog(
        VBox(
          PushButton(Id(1), "&Do Something 1..."),
          PushButton(Id(2), "&Do Something 2..."),
          PushButton(Id(3), "&Do Something 3..."),
          PushButton(Id(:quit), "&Quit")
        )
      )


      @button = nil
      @count = 3
      begin
        @button = UI.UserInput

        if @button != :quit
          @count = Ops.add(@count, 1)
          @label = Builtins.sformat("&Do Something %1...", @count)
          Builtins.y2milestone(
            "Changing button label for button #%1 to \"%2\"",
            @button,
            @label
          )
          UI.ChangeWidget(Id(@button), :Label, @label)
        end
      end while @button != :quit


      UI.CloseDialog

      nil
    end
  end
end

Yast::ShortcutConflict2Client.new.main
