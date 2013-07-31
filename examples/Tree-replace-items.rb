# encoding: utf-8

# Example showing how to replace Tree items
module Yast
  class TreeReplaceItemsClient < Client
    def main
      Yast.import "UI"

      @pizza_list = [
        "Pizza Napoli",
        "Pizza Funghi",
        "Pizza Salami",
        "Pizza Hawaii"
      ]

      @pasta_list = ["Spaghetti", "Rigatoni", "Tortellini"]

      @veggie_toppings = [
        "Cheese",
        "Mushrooms",
        "Pepperoni",
        "Rucola",
        "Tomatoes"
      ]

      @meat_toppings = ["Ham", "Salami", "Tuna"]

      @menu = [
        Item(Id(:pizza_branch), "Pizza", true, @pizza_list),
        Item(Id(:pasta_branch), "Pasta", true, @pasta_list)
      ]

      @toppings = [
        Item(Id(:meat_branch), "Meat", true, @meat_toppings),
        Item(Id(:veggie_branch), "Veggie", true, @veggie_toppings)
      ]


      UI.OpenDialog(
        VBox(
          Tree(Id(:listing), "Daily &Specials:", @menu),
          HBox(
            PushButton(Id(:menu), "&Menu"),
            PushButton(Id(:toppings), "&Toppings"),
            PushButton(Id(:empty), "&None")
          ),
          PushButton(Id(:ok), "&OK")
        )
      )

      @button = nil
      begin
        @button = Convert.to_symbol(UI.UserInput)

        UI.ChangeWidget(:listing, :Items, @menu) if @button == :menu
        UI.ChangeWidget(:listing, :Items, @toppings) if @button == :toppings
        UI.ChangeWidget(:listing, :Items, []) if @button == :empty
      end while @button != :ok

      @order = Convert.to_string(UI.QueryWidget(:listing, :CurrentItem))
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

Yast::TreeReplaceItemsClient.new.main
