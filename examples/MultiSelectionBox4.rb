# encoding: utf-8

module Yast
  class MultiSelectionBox4Client < Client
    def main
      Yast.import "UI"
      Yast.import "Label"

      @items = [
        Item(Id("a"), "a"),
        Item(Id("x"), "x"),
        Item(Id("y"), "y"),
        Item(Id("z"), "z")
      ]

      @contents = HBox(
        VSpacing(20),
        VBox(
          HSpacing(70),
          VSpacing(0.2),
          MultiSelectionBox(
            Id(:multisel),
            Opt(:notify),
            "Multiselection",
            @items
          ),
          HBox(
            PushButton(Id(:ok), Opt(:default), Label.OKButton),
            PushButton(Id(:cancel), Opt(:key_F9), Label.CancelButton)
          ),
          VSpacing(0.2)
        )
      )

      UI.OpenDialog(Opt(:decorated), @contents)
      UI.ChangeWidget(:multisel, :CurrentItem, nil)

      UI.ChangeWidget(:multisel, :SelectedItems, ["a", "x"])
      UI.ChangeWidget(:multisel, :SelectedItems, ["y", "z"])

      @selected_items = Convert.to_list(
        UI.QueryWidget(Id(:multisel), :SelectedItems)
      )
      Builtins.y2milestone("Selected items: %1", @selected_items)
      Builtins.y2milestone(
        "Current item: %1",
        Convert.to_string(UI.QueryWidget(Id(:multisel), :CurrentItem))
      )
      while true
        @event = UI.WaitForEvent
        @ret = Ops.get(@event, "ID")
        if @ret == :multisel
          @current2 = Convert.to_string(
            UI.QueryWidget(Id(:multisel), :CurrentItem)
          )
          Builtins.y2internal("current: %1", @current2)
        end

        break if @ret == :ok || @ret == :cancel
      end
      @current = Convert.to_string(UI.QueryWidget(Id(:multisel), :CurrentItem))
      Builtins.y2internal("current before leaving: %1", @current)
      UI.CloseDialog

      nil
    end
  end
end

Yast::MultiSelectionBox4Client.new.main
