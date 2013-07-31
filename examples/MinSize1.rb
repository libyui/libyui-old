# encoding: utf-8

# Simple example for MinSize
module Yast
  class MinSize1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          MinSize(
            50,
            12,
            RichText(
              "<h3MinSize example</h3>" +
                "<p>MinSize is particularly useful in connection with widgets" +
                " that can scroll, such as" + "<ul>" + "<li>RichText" + "<li>SelectionBox" + "<li>Table" +
                "<li>MultiLineEdit" + "</ul>" +
                "since those widgets don't have a reasonable default size." + "</p>"
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

Yast::MinSize1Client.new.main
