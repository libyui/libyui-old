# encoding: utf-8

# Example for a RichText widget
module Yast
  class RichText1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          RichText(
            "<h3>RichText example</h3>" +
              "<p>This is a <i>RichText</i> widget.</p>" +
              "<p>It's very much like <i>HTML</i>, but not quite as powerful.</p>" +
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
              "<font color=red>\tUI.\t\t</font>" + "</p>" +
              "<p>The product name is automatically replaced by the UI. " +
              "Use the special macro <b>&amp;product;</b> for that." + "</p><p>" +
              "The current product name is <b>&product;</b>." + "</p>"
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

Yast::RichText1Client.new.main
