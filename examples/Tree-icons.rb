# encoding: utf-8

# Tree with icons
module Yast
  class TreeIconsClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Heading("YaST2 Mini Control Center"),
          Tree(
            Id(:mod),
            "Modules",
            [
              Item(
                Id("country"),
                term(:icon, "yast-language.png"),
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
              Item(
                Id("sw_single"),
                term(:icon, "yast-software.png"),
                "Software"
              )
            ]
          ),
          PushButton(Id(:ok), Opt(:default), "&OK")
        )
      )

      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::TreeIconsClient.new.main
