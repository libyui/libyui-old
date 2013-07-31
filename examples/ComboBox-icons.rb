# encoding: utf-8

# Combo box with icons
module Yast
  class ComboBoxIconsClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Heading("YaST2 Mini Control Center"),
          ComboBox(
            Id(:mod),
            "Modules",
            [
              Item(Id("keyboard"), term(:icon, "yast-keyboard.png"), "Keyboard"),
              Item(Id("mouse"), term(:icon, "yast-mouse.png"), "Mouse"),
              Item(
                Id("timezone"),
                term(:icon, "yast-timezone.png"),
                "Time zone"
              ),
              Item(Id("lan"), term(:icon, "yast-lan.png"), "Network"),
              Item(
                Id("sw_single"),
                term(:icon, "yast-software.png"),
                "Software"
              )
            ]
          ),
          PushButton("&OK")
        )
      )
      UI.UserInput

      # Get the input from the combo box.
      #
      # Notice: The return value of UI::UserInput() does NOT return this value!
      # Rather, it returns the ID of the widget (normally the PushButton)
      # that caused UI::UserInput() to return.
      @mod = Convert.to_string(UI.QueryWidget(Id(:mod), :Value))

      # Close the dialog.
      # Remember to read values from the dialog's widgets BEFORE closing it!
      UI.CloseDialog

      nil
    end
  end
end

Yast::ComboBoxIconsClient.new.main
