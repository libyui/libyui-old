# encoding: utf-8

# Advanced MultiSelectionBox example:
#
# Get and change the current item. Note that unlike with a SelectionBox, this
# is typically NOT what an application needs: The current item is the item
# that has the keyboard focus. It doesn't need to be selected. Most
# applications will want to use SelectedItems rather than CurrentItem.
module Yast
  class MultiSelectionBoxCurrentItemClient < Client
    def main
      Yast.import "UI"

      UI.OpenDialog(
        VBox(
          MultiSelectionBox(
            Id(:toppings),
            "Select pizza toppings:",
            [
              Item(Id(0), "Cheese", true),
              Item(Id(1), "Tomatoes", true),
              Item(Id(2), "Mushrooms", false),
              Item(Id(3), "Onions"),
              Item(Id(4), "Salami"),
              Item(Id(5), "Ham")
            ]
          ),
          HBox(
            PushButton(Id(:next), "&Next"),
            HStretch(),
            PushButton(Id(:cancel), "&Close")
          )
        )
      )

      @widget = nil
      begin
        @widget = UI.UserInput

        if @widget == :next
          @current = Convert.to_integer(UI.QueryWidget(:toppings, :CurrentItem))

          @current = Ops.add(@current, 1)
          @current = 0 if Ops.greater_than(@current, 5)

          Builtins.y2milestone("Current: %1", @current)
          UI.ChangeWidget(:toppings, :CurrentItem, @current)
        end
      end until @widget == :cancel

      UI.CloseDialog

      nil
    end
  end
end

Yast::MultiSelectionBoxCurrentItemClient.new.main
