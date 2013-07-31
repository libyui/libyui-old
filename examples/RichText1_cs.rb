# encoding: utf-8

# Example for a RichText widget
module Yast
  class RichText1CsClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          RichText(
            "<h3>RichText example</h3>" +
              "<p>This is a <i>RichText</i> widget.</p>" +
              "<p>It's czech: \u011B\u0161\u010D\u0159\u017E\u00FD\u00E1\u00ED\u02C7<i>HTML</i>, or german \u00F6\u00F6\u00E4\u00E4\u00E4</p>" +
              "<p><b>bold</b> and <i>italic</i> you can rely on.</p>" + "<p>" +
              "<font color=blue>colored\t</font>" +
              "<font color=red>\ttext \t</font>" +
              "<font color=green>\tmight \t</font>" +
              "<font color=magenta>\tor \t</font>" +
              "<font color=cyan>\tmight \t</font>" +
              "<font color=blue>\tnot\t</font>" +
              "<font color=red>\tbe\t</font>" +
              "<font color=green>\tavailable,</font>" +
              "<font color=magenta>\tdepending\t</font>" +
              "<font color=cyan>\ton\t\t</font>" +
              "<font color=blue>\tthe\t\t</font>" +
              "<font color=red>\tUI.\t\t</font>" + "</p>"
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

Yast::RichText1CsClient.new.main
