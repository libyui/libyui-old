# encoding: utf-8

# Example showing how to replace SelectionBox items
module Yast
  class SelectionBoxReplaceItems2Client < Client
    def main
      Yast.import "UI"

      @pizza_list = [
        Item(Id("Pizza #01"), "Pizza Napoli"),
        Item(Id("Pizza #02"), "Pizza Funghi"),
        Item(Id("Pizza #03"), "Pizza Salami", true),
        Item(Id("Pizza #04"), "Pizza Hawaii")
      ]

      @pasta_list = [
        Item(Id("Pasta #11"), "Spaghetti"),
        Item(Id("Pasta #12"), "Rigatoni", true),
        Item(Id("Pasta #13"), "Tortellini")
      ]

      UI.OpenDialog(
        VBox(
          SelectionBox(Id(:menu), "Daily &Specials:", @pizza_list),
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

      @order = Convert.to_string(UI.QueryWidget(:menu, :CurrentItem))
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

Yast::SelectionBoxReplaceItems2Client.new.main
