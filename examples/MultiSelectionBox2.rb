# encoding: utf-8

module Yast
  class MultiSelectionBox2Client < Client
    def main
      Yast.import "UI"
      # More realistic MultiSelectionBox example:
      #
      # Items have IDs, some are preselected.
      # Notice 'false' is default anyway for the selection state,
      # so you may or may not explicitly specify that.

      UI.OpenDialog(
        VBox(
          MultiSelectionBox(
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
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::MultiSelectionBox2Client.new.main
