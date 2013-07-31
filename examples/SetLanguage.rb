# encoding: utf-8

# -*- coding: utf-8 -*-
module Yast
  class SetLanguageClient < Client
    def main
      Yast.import "UI"
      # Example for UI::SetLanguage() UI::GetLanguage()

      UI.OpenDialog(
        VBox(
          InputField(Id(:lang_field), "&Language:"),
          HBox(
            Label("Current language:"),
            Label(
              Id(:lang_label),
              Opt(:outputField, :hstretch),
              UI.GetLanguage(false)
            )
          ),
          HBox(
            PushButton(Id(:set), "UI::&SetLanguage()\n"),
            PushButton(Id(:get_strip), "UI::&GetLanguage()\nstrip encoding"),
            PushButton(Id(:get_nostrip), "&UI::GetLanguage()\n&with encoding"),
            PushButton(Id(:test_dialog), "&Test Dialog"),
            HSpacing(3),
            PushButton(Id(:close), "&Close")
          )
        )
      )

      UI.SetFocus(Id(:lang_field))

      while true
        @id = UI.UserInput

        if @id == :close
          UI.CloseDialog
          return
        elsif @id == :get_strip
          UI.ChangeWidget(Id(:lang_field), :Value, UI.GetLanguage(true))
        elsif @id == :get_nostrip
          UI.ChangeWidget(Id(:lang_field), :Value, UI.GetLanguage(false))
        elsif @id == :set
          UI.SetLanguage(
            Convert.to_string(UI.QueryWidget(Id(:lang_field), :Value))
          )
          UI.ChangeWidget(Id(:lang_label), :Value, UI.GetLanguage(false))
        elsif @id == :test_dialog
          UI.OpenDialog(
            VBox(
              Heading("Test Dialog"),
              Label(
                "Fonts may have changed\n" +
                  "Gr\u00FC\u00DF Gott\n" +
                  "Czech (\u010Desky)\n" +
                  "Greek (\u0395\u03BB\u03BB\u03B7\u03BD\u03B9\u03BA\u03AC)\n" +
                  "Russian (\u0420\u0443\u0441\u0441\u043A\u0438\u0439)\n" +
                  "\u6F22\u5B57\u304B\u306A\u76F4\n" +
                  "Hangul (\uD55C\uAE00)"
              ),
              PushButton(Opt(:default), "&OK")
            )
          )
          UI.UserInput
          UI.CloseDialog
        end
      end

      nil
    end
  end
end

Yast::SetLanguageClient.new.main
