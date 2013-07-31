# encoding: utf-8

# Input field width under different layout constraints
module Yast
  class InputFieldLayoutClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          InputField("Normal", "normal width"),
          InputField(Opt(:shrinkable), "Sml", "shrinkable"),
          VBox(
            MarginBox(
              1,
              0.2,
              Frame(
                "Useless Frame",
                VBox(
                  Heading("Very Wide and Very Useless Heading"),
                  InputField("Normal 1"),
                  InputField("Normal 2"),
                  InputField("Normal 3"),
                  InputField("Unusually &wide field caption to make it wider"),
                  InputField("Normal 4"),
                  InputField(Opt(:hstretch), "Stretch", "stretchable"),
                  InputField(Opt(:shrinkable), "Sml", "shrinkable"),
                  Left(InputField("Left", "left aligned")),
                  HSquash(InputField("HSquashed", "hquashed"))
                )
              )
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

Yast::InputFieldLayoutClient.new.main
