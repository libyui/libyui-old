# encoding: utf-8

module Yast
  class CheckBox4Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Label("Select your extras"),
          Left(CheckBox(Id(:cheese), "Extra Cheese")),
          Left(CheckBox(Id(:pepr), "Pepperoni", true)),
          Left(CheckBox(Id(:salami), Opt(:boldFont), "Extra Salami")),
          PushButton("&OK")
        )
      )
      UI.UserInput
      @cheese = Convert.to_boolean(UI.QueryWidget(:cheese, :Value))
      @pepr = Convert.to_boolean(UI.QueryWidget(:pepr, :Value))
      @salami = Convert.to_boolean(UI.QueryWidget(:salami, :Value))
      UI.CloseDialog

      UI.OpenDialog(
        VBox(
          Left(Label(Ops.add("Extra Cheese: ", yesno(@cheese)))),
          Left(Label(Ops.add("Pepperoni: ", yesno(@pepr)))),
          Left(Label(Ops.add("Extra Salami: ", yesno(@salami)))),
          PushButton("&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end

    def yesno(b)
      if b
        return "yes"
      else
        return "no"
      end
    end
  end
end

Yast::CheckBox4Client.new.main
