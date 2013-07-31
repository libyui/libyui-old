# encoding: utf-8

# Advanced ComboBox / SelecionBox example:
# Dialog with a ComboBox and a SelectionBox
# where selecting an item in one will also select it in the other
module Yast
  class ComboBox5Client < Client
    def main
      Yast.import "UI"
      @items = [
        Item(Id("Napoli"), "Napoli"),
        Item(Id("Funghi"), "Funghi", true),
        Item(Id("Salami"), "Salami")
      ]

      UI.OpenDialog(
        VBox(
          MinSize(
            30,
            5,
            HBox(
              Top(ComboBox(Id(:pizzaComboBox), Opt(:notify), "Pizza:", @items)),
              SelectionBox(Id(:pizzaSelBox), Opt(:notify), "Pizza:", @items)
            )
          ),
          Right(PushButton(Id(:cancel), "&Close"))
        )
      )

      @widget = nil
      begin
        @widget = Convert.to_symbol(UI.UserInput)
        # y2debug( "Event widget: %1", widget );

        if @widget == :pizzaComboBox || @widget == :pizzaSelBox
          @pizza = Convert.to_string(UI.QueryWidget(@widget, :Value))
          Builtins.y2debug("Pizza: %1", @pizza)

          UI.ChangeWidget(
            @widget == :pizzaComboBox ? :pizzaSelBox : :pizzaComboBox,
            :Value,
            @pizza
          )

          Builtins.y2debug(
            "ComboBox: %1",
            UI.QueryWidget(:pizzaComboBox, :Items)
          )
          Builtins.y2debug("SelBox  : %1", UI.QueryWidget(:pizzaSelBox, :Items))
        end
      end until @widget == :cancel

      UI.CloseDialog

      nil
    end
  end
end

Yast::ComboBox5Client.new.main
