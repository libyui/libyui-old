# encoding: utf-8

# Basic example of using the Wizard widget.
#
# Note: YCP applications are discouraged from using the Wizard widget directly.
# Use the Wizard module instead.
module Yast
  class Wizard1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        Wizard(:back, "", :abort, "&Cancel", :next, "&OK")
      )

      while true
        @event = UI.WaitForEvent

        Builtins.y2milestone("Got event: %1", @event)

        break if Ops.get(@event, "ID") == :abort
      end

      UI.DumpWidgetTree
      UI.CloseDialog

      nil
    end
  end
end

Yast::Wizard1Client.new.main
