# encoding: utf-8

# Advanced MultiSelectionBox example:
#
# Retrieve the list of selected items and output it.
module Yast
  class MultiSelectionBox5Client < Client
    def main
      Yast.import "UI"

      UI.OpenDialog(
        VBox(
          Heading("Today's menu"),
          MinSize(
            55,
            10,
            HBox(
              Table(
                Id(:pizza),
                Opt(:notify),
                Header("Pizza", "Price"),
                [
                  Item(Id(:vege), "Vegetarian", 6),
                  Item(Id(:capri), "Capricciosa", 7),
                  Item(Id(:meat), "Meat and more", 8)
                ]
              ),
              MultiSelectionBox(
                Id(:toppings),
                "Select pizza toppings:",
                [
                  Item(Id(:cheese), "Cheese", true),
                  Item(Id(:tomatoes), "Tomatoes", true),
                  Item(Id(:mushroom), "Mushrooms", false),
                  Item(Id(:onions), "Onions")
                ]
              )
            )
          ),
          PushButton(Id(:ok), Opt(:default), "&OK")
        )
      )

      @vege = [
        Item(Id(:cheese), "Cheese", true),
        Item(Id(:tomatoes), "Tomatoes", true),
        Item(Id(:mushroom), "Mushrooms", false),
        Item(Id(:onions), "Onions")
      ]
      @meat = [Item(Id(:sausage), "Salami", true), Item(Id(:pork), "Ham")]
      @capri = Builtins.merge(@vege, @meat)

      @ret = nil
      begin
        @ret = UI.UserInput

        if @ret == :pizza
          @item = UI.QueryWidget(:pizza, :CurrentItem)
          if @item == :vege
            UI.ChangeWidget(:toppings, :Items, @vege)
          elsif @item == :capri
            UI.ChangeWidget(:toppings, :Items, @capri)
          elsif @item == :meat
            UI.ChangeWidget(:toppings, :Items, @meat)
          end
        end
      end until @ret == :ok

      @selected_items = Convert.to_list(
        UI.QueryWidget(Id(:toppings), :SelectedItems)
      )

      Builtins.y2debug("Selected items: %1", @selected_items)

      # Remember to retrieve the widget's data _before_ the dialog is closed,
      # i.e. before it is destroyed!

      UI.CloseDialog



      # Concatenate the list of selected toppings to one multi-line string.

      @pizza_description = ""

      Builtins.foreach(@selected_items) do |topping|
        @pizza_description = Builtins.sformat(
          "%1\n%2",
          @pizza_description,
          topping
        )
      end


      # Open a new dialog to echo the selection.

      UI.OpenDialog(
        VBox(
          Label("Your pizza will come with:\n"),
          Label(@pizza_description),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::MultiSelectionBox5Client.new.main
