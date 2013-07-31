# encoding: utf-8

# Tree with recursive multi selection
module Yast
  class TreeCheckbox4Client < Client
    def main
      Yast.import "UI"



      UI.OpenDialog(
        VBox(
          Heading("YaST2 Mini Control Center"),
          Tree(
            Id(:mod),
            Opt(:multiSelection, :notify, :immediate, :recursiveSelection),
            "Modules",
            [
              Item(Id("unselected"), "Unseleted"),
              Item(
                Id("country"),
                "Localization",
                true,
                [
                  Item(Id("keyboard"), "Keyboard"),
                  Item(
                    Id("timezone"),
                    "Time zone",
                    true,
                    [Item(Id(:europe), "Europe"), Item(Id(:asia), "Asia")]
                  )
                ]
              ),
              Item(Id(:mouse), "Mouse"),
              Item(Id(:lan), "Network"),
              Item(Id(:xmas), "Merry X-Mas"),
              Item(Id(:newyear), "Happy New Year")
            ]
          ),
          PushButton(Id(:ok), Opt(:default), "&OK")
        )
      )

      UI.ChangeWidget(:mod, :SelectedItems, [:lan, :mouse])
      UI.ChangeWidget(:mod, :SelectedItems, [:xmas, :newyear])

      @selected_items = Convert.to_list(
        UI.QueryWidget(Id(:mod), :SelectedItems)
      )
      Builtins.y2warning("Selected items: %1", @selected_items)


      @id = nil

      @event = {}
      begin
        @event = UI.WaitForEvent(1000 * 100)

        if Ops.get(@event, "EventReason") == "SelectionChanged"
          Builtins.y2error("Selection Changed Event")
        end

        if Ops.get(@event, "EventReason") == "ValueChanged"
          Builtins.y2error("Value Changed Event")
        end

        if Ops.get(@event, "EventType") == "TimeoutEvent"
          Builtins.y2error("Timeout Event")
        end

        Builtins.y2error(formatEvent(@event)) if @event != nil

        @id = Ops.get(@event, "ID")
        Builtins.y2milestone(
          "Selected items: %1",
          Convert.to_list(UI.QueryWidget(Id(:mod), :SelectedItems))
        )
      end until @id == :ok

      nil
    end

    def formatEvent(event)
      event = deep_copy(event)
      html = "Event:"
      Builtins.foreach(event) do |key, value|
        html = Ops.add(
          Ops.add(
            Ops.add(Ops.add(Ops.add(html, " "), key), ": "),
            Builtins.tostring(value)
          ),
          ""
        )
      end

      html
    end
  end
end

Yast::TreeCheckbox4Client.new.main
