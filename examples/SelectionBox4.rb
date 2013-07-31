# encoding: utf-8

module Yast
  class SelectionBox4Client < Client
    def main
      Yast.import "UI"
      # Create a selection box with three entries.
      # All entries have IDs to identify them independent of locale
      # (The texts might have to be translated!).
      # Entry "Funghi" will be selected by default.
      #
      # There are two buttons to select a "Today's special" and a
      # "veggie" pizza to demonstrate how to select list entries
      # from within a YCP script - even without having to use item IDs.
      UI.OpenDialog(
        VBox(
          SelectionBox(
            Id(:pizza),
            "Select your Pizza:",
            [
              "Napoli",
              "Funghi",
              "Salami",
              "Quattro Stagioni (a pizza which is devided into 4 parts each with a different topping)",
              "Caprese",
              "Speciale",
              "Hawaii"
            ]
          ),
          HBox(
            PushButton(Id(:todays_special), Opt(:hstretch), "&Today's special"),
            PushButton(Id(:veggie), Opt(:hstretch), "&Veggie")
          ),
          PushButton(Id(:ok), Opt(:default), "&OK")
        )
      )

      @id = nil
      begin
        @id = UI.UserInput

        if @id == :todays_special
          UI.ChangeWidget(Id(:pizza), :CurrentItem, "Napoli")
        elsif @id == :veggie
          UI.ChangeWidget(Id(:pizza), :CurrentItem, "Funghi")
        end
      end until @id == :ok

      # Get the input from the selection box.
      #
      # Notice: The return value of UI::UserInput() does NOT return this value!
      # Rather, it returns the ID of the widget (normally the PushButton)
      # that caused UI::UserInput() to return.
      @pizza = Convert.to_string(UI.QueryWidget(Id(:pizza), :CurrentItem))

      # Close the dialog.
      # Remember to read values from the dialog's widgets BEFORE closing it!
      UI.CloseDialog


      # Pop up a new dialog to echo the selection.
      UI.OpenDialog(
        VBox(
          Label(Ops.add(Ops.add("Pizza ", @pizza), " coming right up")),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput

      UI.CloseDialog

      nil
    end
  end
end

Yast::SelectionBox4Client.new.main
