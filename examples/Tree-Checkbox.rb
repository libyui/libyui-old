# encoding: utf-8

# Tree with icons
module Yast
  class TreeCheckboxClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Heading("YaST2 Mini Control Center"),
          Tree(
            Id(:mod),
            Opt(:multiSelection),
            "Modules",
            [
              Item(
                Id("country"),
                term(:icon, "yast-yast-language.png"),
                "Localization",
                true,
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
              Item(Id(:xmas), term(:icon, "yast-software.png"), "Merry X-Mas"),
              Item(
                Id(:newyear),
                term(:icon, "yast-software.png"),
                "Happy New Year"
              )
            ]
          ),
          PushButton(Id(:ok), Opt(:default), "&OK")
        )
      )

      UI.ChangeWidget(:mod, :SelectedItems, [:xmas, :newyear])

      @id = nil
      begin
        @id = UI.TimeoutUserInput(300)
        @selected_items = Convert.to_list(
          UI.QueryWidget(Id(:mod), :SelectedItems)
        )
        Builtins.y2warning("Selected items: %1", @selected_items)
      end until @id == :ok

      nil
    end
  end
end

Yast::TreeCheckboxClient.new.main
