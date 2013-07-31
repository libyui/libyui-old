# encoding: utf-8

module Yast
  class Table8MultiSelClient < Client
    def main
      Yast.import "UI"
      @new_items = [
        Item(Id(1), "Mercedes", 60000),
        Item(Id(2), "Audi", 50000),
        Item(Id(3), "VW", 40000),
        Item(Id(4), "BMW", 60000),
        Item(Id(5), "Porsche", 80000)
      ]
      @orig_items = [
        Item(Id(1), "Chili", 6),
        Item(Id(2), "Salami Baguette", nil),
        Item(Id(3), "Spaghetti", 8),
        Item(Id(4), "Steak Sandwich", 12)
      ]

      UI.OpenDialog(
        VBox(
          Heading("Today's menu"),
          MinSize(
            30,
            10,
            Table(
              Id(:menu),
              Opt(:notify, :multiSelection),
              Header("Name", "Price"),
              [
                Item(Id(1), "Chili", 6),
                Item(Id(2), "Salami Baguette", nil),
                Item(Id(3), "Spaghetti", 8),
                Item(Id(4), "Steak Sandwich", 12)
              ]
            )
          ),
          Label("Get notified on 'Return' or double click"),
          HBox(Label("Selected: "), TextEntry(Id(:info), "")),
          HBox(
            PushButton(Id(:next), "Change &Table Contents"),
            PushButton(Id(:cancel), "&OK")
          )
        )
      )

      UI.ChangeWidget(:menu, :SelectedItems, [1, 2])
      @sel = Convert.convert(
        UI.QueryWidget(:menu, :SelectedItems),
        :from => "any",
        :to   => "list <integer>"
      )
      @selItems = ""
      Builtins.foreach(@sel) do |val|
        @selItems = Ops.add(Ops.add(@selItems, " "), Builtins.tostring(val))
      end

      UI.ChangeWidget(:info, :Value, @selItems)

      @event = {}
      @num = 0
      begin
        @selItems2 = ""
        @event = UI.WaitForEvent

        if Ops.get(@event, "ID") == :menu
          @sel = Convert.convert(
            UI.QueryWidget(:menu, :SelectedItems),
            :from => "any",
            :to   => "list <integer>"
          )
          Builtins.foreach(@sel) do |val|
            @selItems2 = Ops.add(
              Ops.add(@selItems2, " "),
              Builtins.tostring(val)
            )
          end
          UI.ChangeWidget(:info, :Value, @selItems2)
        elsif Ops.get(@event, "ID") == :next
          @num = Ops.add(@num, 1)
          @items = []
          if Ops.modulo(@num, 2) == 1
            @items = deep_copy(@new_items)
          else
            @items = deep_copy(@orig_items)
          end

          # Change table contents
          UI.ChangeWidget(:menu, :Items, @items)
          @sel = Convert.convert(
            UI.QueryWidget(:menu, :SelectedItems),
            :from => "any",
            :to   => "list <integer>"
          )
          Builtins.foreach(@sel) do |val|
            @selItems2 = Ops.add(
              Ops.add(@selItems2, " "),
              Builtins.tostring(val)
            )
          end
          UI.ChangeWidget(:info, :Value, @selItems2)

          # Double check: Retrieve contents and dump to log
          Builtins.y2milestone(
            "New table content:\n%1",
            UI.QueryWidget(:menu, :Items)
          )
        end
      end until Ops.get(@event, "ID") == :cancel

      @sel = Convert.convert(
        UI.QueryWidget(:menu, :SelectedItems),
        :from => "any",
        :to   => "list <integer>"
      )
      Builtins.y2milestone("Selected: %1", @sel)

      UI.CloseDialog

      nil
    end
  end
end

Yast::Table8MultiSelClient.new.main
