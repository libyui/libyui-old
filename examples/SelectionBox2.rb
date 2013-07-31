# encoding: utf-8

# Simple SelectionBox example
module Yast
  class SelectionBox2Client < Client
    def main
      Yast.import "UI"
      # Create a selection box with three entries.
      # All entries have IDs to identify them independent of locale
      # (The texts might have to be translated!).
      # Entry "Funghi" will be selected by default.
      UI.OpenDialog(
        VBox(
          SelectionBox(
            Id(:pizza),
            "Select your Pizza:",
            [
              Item(Id(:nap), "Napoli"),
              Item(Id(:fun), "Funghi", true),
              Item(Id(:sal), "Salami")
            ]
          ),
          PushButton("&OK")
        )
      )
      UI.UserInput

      # Get the input from the selection box.
      #
      # Notice: The return value of UI::UserInput() does NOT return this value!
      # Rather, it returns the ID of the widget (normally the PushButton)
      # that caused UI::UserInput() to return.
      @pizza = UI.QueryWidget(Id(:pizza), :CurrentItem)
      Builtins.y2milestone("Selected pizza: %1", @pizza)

      # Close the dialog.
      # Remember to read values from the dialog's widgets BEFORE closing it!
      UI.CloseDialog

      # Evaluate selection

      @toppings = "nothing"

      if @pizza == :nap
        @toppings = "Tomatoes, Cheese"
      elsif @pizza == :fun
        @toppings = "Tomatoes, Cheese, Mushrooms"
      elsif @pizza == :sal
        @toppings = "Tomatoes, Cheese, Sausage"
      end

      # Pop up a new dialog to echo the selection.
      UI.OpenDialog(
        VBox(
          Label("You will get a pizza with:"),
          Label(@toppings),
          PushButton("&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::SelectionBox2Client.new.main
