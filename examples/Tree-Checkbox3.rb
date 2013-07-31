# encoding: utf-8

module Yast
  class TreeCheckbox3Client < Client
    def main
      Yast.import "UI"
      Yast.import "Label"

      @items = [
        Item(Id("a"), "a", false, []),
        Item(
          Id("x"),
          "x",
          true,
          [Item(Id("y"), "y", false, []), Item(Id("z"), "z", false, [])]
        )
      ]

      @contents = HBox(
        VSpacing(20),
        VBox(
          HSpacing(70),
          VSpacing(0.2),
          #`Tree (`id(`tree), `opt(`notify, `immediate, `multiSelection),  "tree", items),
          Tree(Id(:tree), Opt(:notify, :multiSelection), "tree", @items),
          #`Tree (`id(`tree), `opt(`notify),  "tree", items),
          HBox(
            PushButton(Id(:ok), Opt(:default), Label.OKButton),
            PushButton(Id(:cancel), Opt(:key_F9), Label.CancelButton)
          ),
          VSpacing(0.2)
        )
      )

      UI.OpenDialog(Opt(:decorated), @contents)
      UI.ChangeWidget(:tree, :CurrentItem, nil)
      while true
        @event = UI.WaitForEvent
        @ret = Ops.get(@event, "ID")
        if @ret == :tree
          @current2 = Convert.to_string(UI.QueryWidget(Id(:tree), :CurrentItem))
          Builtins.y2internal("current: %1", @current2)
        end

        break if @ret == :ok || @ret == :cancel
      end
      @current = Convert.to_string(UI.QueryWidget(Id(:tree), :CurrentItem))
      Builtins.y2internal("current before leaving: %1", @current)

      UI.CloseDialog

      nil
    end
  end
end

Yast::TreeCheckbox3Client.new.main
