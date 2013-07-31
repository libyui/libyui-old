# encoding: utf-8

# PushButton with icons (relative path)
module Yast
  class IconButton1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Heading("YaST2 Mini Control Center"),
          IconButton(Id("keyboard "), "yast-keyboard.png", "Keyboard"),
          IconButton(Id("mouse"), "yast-mouse.png", "Mouse"),
          IconButton(Id("timezone"), "yast-timezone.png", "Time zone"),
          IconButton(Id("lan"), "yast-lan.png", "Network"),
          IconButton(Id("sw_single"), "yast-software.png", "Software")
        )
      )

      @ret = nil
      begin
        @ret = UI.UserInput

        if @ret != :cancel
          UI.OpenDialog(
            Label(Ops.add(Ops.add("Running ", Convert.to_string(@ret)), "..."))
          )
          Builtins.sleep(4000)
          UI.CloseDialog
        end
      end while @ret != :cancel

      UI.CloseDialog

      nil
    end
  end
end

Yast::IconButton1Client.new.main
