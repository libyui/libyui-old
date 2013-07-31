# encoding: utf-8

# Example showing how to replace SelectionBox items
module Yast
  class ComboBoxReplaceItems1Client < Client
    def main
      Yast.import "UI"

      @pizza_list = [
        "Pizza Napoli",
        "Pizza Funghi",
        "Pizza Salami",
        "Pizza Hawaii"
      ]

      @pasta_list = ["Spaghetti", "Rigatoni", "Tortellini"]

      UI.OpenDialog(
        VBox(
          ComboBox(Id(:menu), "Daily &Specials:", @pizza_list),
          HBox(
            PushButton(Id(:pizza), "Pi&zza"),
            PushButton(Id(:pasta), "&Pasta")
          ),
          PushButton(Id(:ok), "&OK")
        )
      )

      @button = nil
      begin
        @button = Convert.to_symbol(UI.UserInput)

        UI.ChangeWidget(:menu, :Items, @pizza_list) if @button == :pizza
        UI.ChangeWidget(:menu, :Items, @pasta_list) if @button == :pasta
      end while @button != :ok

      @order = Convert.to_string(UI.QueryWidget(:menu, :Value))
      UI.CloseDialog


      #
      # Show the result
      #

      UI.OpenDialog(
        VBox(
          Label(Builtins.sformat("Your order: %1", @order)),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::ComboBoxReplaceItems1Client.new.main
