# encoding: utf-8

# Example for a RichText widget
module Yast
  class RichText2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          RichText(
            Opt(:plainText),
            "This is a RichText in   plainText   mode.\n" +
              "No HTML \t\ttags\tare\tsupported\there, tabs\tand\tline\tbreaks\n" +
              "are output literally \n" +
              "as are HTML tags like <b> or <i> or &product;."
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

Yast::RichText2Client.new.main
