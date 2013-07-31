# encoding: utf-8

# Example for a RichText widget
module Yast
  class RichTextLicenseClient < Client
    def main
      Yast.import "UI"
      Yast.import "String"

      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          Label("HTML text or text in <pre>...</pre> tags"),
          RichText(Id(:text), ""),
          Label("Plain text (`opt(`plainText))"),
          RichText(Id(:plaintext), Opt(:plainText), ""),
          HBox(
            PushButton(Id(:load), "&Load File"),
            PushButton(Id(:close), "&Close")
          )
        )
      )
      @button = nil
      @name = ""
      @text = ""
      begin
        @button = UI.UserInput

        if @button == :load
          @name = UI.AskForExistingFile(".", "*.txt *orig", "Select text file")
          @text2 = Convert.to_string(SCR.Read(path(".target.string"), @name))

          @text2 = "" if @text2 == nil

          if Builtins.regexpmatch(@text2, "</.*>")
            # HTML text (or RichText)
            UI.ChangeWidget(Id(:text), :Value, @text2)
          else
            # plain text
            UI.ChangeWidget(
              Id(:text),
              :Value,
              Ops.add(Ops.add("<pre>", String.EscapeTags(@text2)), "</pre>")
            )
          end

          UI.ChangeWidget(Id(:plaintext), :Value, @text2)
        end
      end until @button == :close

      UI.CloseDialog

      nil
    end
  end
end

Yast::RichTextLicenseClient.new.main
