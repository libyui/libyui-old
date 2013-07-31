# encoding: utf-8

# Example showing how to replace SelectionBox items
module Yast
  class MultiSelectionBoxReplaceItems2Client < Client
    def main
      Yast.import "UI"

      @all_toppings = [
        Item(Id("Cheese"), "Cheese", true),
        Item(Id("Tomatoes"), "Tomatoes", true),
        Item(Id("Ham"), "Ham"),
        Item(Id("Mushrooms"), "Mushrooms"),
        Item(Id("Pepperoni"), "Pepperoni"),
        Item(Id("Rucola"), "Rucola"),
        Item(Id("Salami"), "Salami"),
        Item(Id("Tuna"), "Tuna")
      ]

      @veggie_toppings = [
        Item(Id("Cheese"), "Cheese", true),
        Item(Id("Tomatoes"), "Tomatoes", true),
        Item(Id("Mushrooms"), "Mushrooms"),
        Item(Id("Pepperoni"), "Pepperoni"),
        Item(Id("Rucola"), "Rucola")
      ]

      UI.OpenDialog(
        HBox(
          VSpacing(15), # layout trick: force minimum height
          VBox(
            HSpacing(25), # force minimum width
            MultiSelectionBox(Id(:toppings), "Toppings:", @all_toppings),
            Left(CheckBox(Id(:veggie), Opt(:notify), "&Vegetarian")),
            PushButton(Id(:ok), "&OK")
          )
        )
      )

      @button = nil
      begin
        @button = Convert.to_symbol(UI.UserInput)

        if @button == :veggie
          @vegetarian = Convert.to_boolean(UI.QueryWidget(:veggie, :Value))

          if @vegetarian
            UI.ChangeWidget(:toppings, :Items, @veggie_toppings)
          else
            UI.ChangeWidget(:toppings, :Items, @all_toppings)
          end
        end
      end while @button != :ok

      @order = Convert.convert(
        UI.QueryWidget(:toppings, :SelectedItems),
        :from => "any",
        :to   => "list <string>"
      )
      UI.CloseDialog


      #
      # Show the result
      #


      UI.OpenDialog(
        VBox(
          Label(
            Builtins.sformat(
              "Your order: %1",
              Builtins.mergestring(@order, ", ")
            )
          ),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::MultiSelectionBoxReplaceItems2Client.new.main
