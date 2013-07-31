# encoding: utf-8

# Create a combo box with three entries.
# All entries have IDs to identify them independent of locale
# (The texts might have to be translated!).
# Entry "Funghi" will be selected by default.
module Yast
  class ComboBoxSetEmptyClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          ComboBox(
            Id(:pizza),
            Opt(:editable),
            "Select your Pizza:",
            [
              Item(Id(:nap), "Napoli"),
              Item(Id(:fun), "Funghi", true),
              Item(Id(:sal), "Salami")
            ]
          ),
          HBox(
            PushButton(Id(:empty), "&Empty"),
            PushButton(Id(42), "42"),
            PushButton(Id(:ok), "&OK")
          )
        )
      )

      @button = nil
      begin
        Builtins.y2debug("Waiting")
        @button = UI.UserInput
        Builtins.y2debug("button: %1", @button)

        UI.ChangeWidget(:pizza, :Value, 42) if @button == 42

        UI.ChangeWidget(:pizza, :Value, "") if @button == :empty
      end until @button == :ok


      # Get the input from the selection box.
      #
      # Notice: The return value of UI::UserInput() does NOT return this value!
      # Rather, it returns the ID of the widget (normally the PushButton)
      # that caused UI::UserInput() to return.
      @pizza = UI.QueryWidget(Id(:pizza), :Value)
      Builtins.y2milestone("Selected %1", @pizza)

      UI.CloseDialog

      nil
    end
  end
end

Yast::ComboBoxSetEmptyClient.new.main
