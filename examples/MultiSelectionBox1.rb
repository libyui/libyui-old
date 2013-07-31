# encoding: utf-8

module Yast
  class MultiSelectionBox1Client < Client
    def main
      Yast.import "UI"
      # Simple MultiSelectionBox example:
      #
      # All items are simple strings, none has an ID, no item preselected.

      UI.OpenDialog(
        VBox(
          MultiSelectionBox(
            "Select pizza toppings:",
            ["Cheese", "Tomatoes", "Mushrooms", "Onions", "Salami", "Ham"]
          ),
          PushButton("&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::MultiSelectionBox1Client.new.main
