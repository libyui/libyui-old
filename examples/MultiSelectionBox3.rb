# encoding: utf-8

# Advanced MultiSelectionBox example:
#
# Retrieve the list of selected items and output it.
module Yast
  class MultiSelectionBox3Client < Client
    def main
      Yast.import "UI"

      UI.OpenDialog(
        VBox(
          MultiSelectionBox(
            Id(:toppings),
            "Select pizza toppings:",
            [
              Item(Id(:cheese), "Cheese", true),
              Item(Id(:tomatoes), "Tomatoes", true),
              Item(Id(:mush), "Mushrooms", false),
              Item(Id(:onions), "Onions"),
              Item(Id(:sausage), "Salami"),
              Item(Id(:pork), "Ham")
            ]
          ),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.ChangeWidget(:toppings, :SelectedItems, [:sausage, :onions])

      UI.UserInput
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

Yast::MultiSelectionBox3Client.new.main
