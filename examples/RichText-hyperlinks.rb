# encoding: utf-8

# Example for a RichText widget with hyperlinks
module Yast
  class RichTextHyperlinksClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          RichText(
            "<h3>RichText example</h3>" +
              "<p>RichText may contain <a href=\"hyper\">hyperlinks</a>," +
              "very much like HTML.</p>" +
              "<p>Clicking on a <a href=\"link\">link</a> will make " +
              "<a href=\"user_input\">UI::UserInput()</a> return " +
              "with the <i>href</i> part of the hyperlink " +
              "as a (string) return value.</p>" +
              "<h3>Known (HTML-like) entities</h3>" + "<ul>" +
              "<li><b>&amp;product;</b> for the product name (<b>&product;</b>)" +
              "<li><b>&amp;amp;</b> for <b>&amp;</b>" +
              "<li><b>&amp;lt;</b> for <b>&lt;</b>" +
              "<li><b>&amp;gt;</b> for <b>&gt;</b>" + "</ul>"
          ),
          HBox(
            Label("You clicked: "),
            Label(Id(:val), Opt(:outputField, :hstretch), "")
          ),
          PushButton(Id(:close), Opt(:default), "&Close")
        )
      )


      @ret = nil
      begin
        @ret = UI.UserInput

        if @ret != nil
          @val = Builtins.sformat("%1", @ret)
          UI.ChangeWidget(Id(:val), :Value, @val)
        end
      end until @ret == :close

      UI.CloseDialog

      nil
    end
  end
end

Yast::RichTextHyperlinksClient.new.main
