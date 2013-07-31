# encoding: utf-8

# Tree with icons
module Yast
  class TreeCheckbox2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Heading("YaST2 Mini Control Center"),
          Tree(
            Id(:mod),
            Opt(:notify, :multiSelection, :immediate),
            "Modules",
            [
              Item(
                Id("country"),
                term(:icon, "yast-yast-language.png"),
                "Localization",
                false,
                [
                  Item(
                    Id("keyboard"),
                    term(:icon, "yast-keyboard.png"),
                    "Keyboard"
                  ),
                  Item(
                    Id("timezone"),
                    term(:icon, "yast-timezone.png"),
                    "Time zone"
                  )
                ]
              ),
              Item(Id("mouse"), term(:icon, "yast-mouse.png"), "Mouse"),
              Item(Id("lan"), term(:icon, "yast-lan.png"), "Network"),
              Item(Id("xmas"), term(:icon, "yast-software.png"), "Merry X-Mas"),
              Item(
                Id("newyear"),
                term(:icon, "yast-software.png"),
                "Happy New Year"
              )
            ]
          ),
          HBox(
            PushButton(Id(:ok), Opt(:default), "&OK"),
            PushButton(Id(:deselect), "&Log selected")
          )
        )
      )

      UI.ChangeWidget(:mod, :SelectedItems, ["keyboard", "xmas", "newyear"])
      @id = nil
      @current = Convert.to_string(UI.QueryWidget(Id(:mod), :CurrentItem))
      Builtins.y2milestone("Current item: %1", @current)
      begin
        @id = UI.UserInput
        @selected_items = Convert.to_list(
          UI.QueryWidget(Id(:mod), :SelectedItems)
        )
        Builtins.y2milestone("Selected items: %1", @selected_items)
        @current = Convert.to_string(UI.QueryWidget(Id(:mod), :CurrentItem))
        Builtins.y2milestone("Current item: %1", @current)
      end until @id == :ok

      nil
    end
  end
end

Yast::TreeCheckbox2Client.new.main
