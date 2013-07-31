# encoding: utf-8

# Multiple Main Dialogs
module Yast
  class MainDialogPopupClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          Heading("Main Dialog"),
          VStretch(),
          PushButton(Id(:doit1), "&Do Something"),
          PushButton(Id(:doit2), "Do &More"),
          PushButton(Id(:popup), "&Popup Main Dialog"),
          VStretch(),
          Right(PushButton(Id(:close), "&Close"))
        )
      )

      while true
        @event = UI.WaitForEvent

        Builtins.y2milestone("Got event: %1", @event)

        @widget = Ops.get_symbol(@event, "ID", :nil)

        break if @widget == :close

        if @widget == :popup
          UI.OpenDialog(
            Opt(:defaultsize),
            VBox(
              Id(:popupDialog),
              Heading("Popup Main Dialog"),
              VStretch(),
              PushButton(Id(:doit11), "&Do Something"),
              PushButton(Id(:doit12), "Do &More"),
              VStretch(),
              Right(PushButton(Id(:popdown), "&Back"))
            )
          )
        end

        UI.CloseDialog if @widget == :popdown && UI.WidgetExists(:popupDialog)
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::MainDialogPopupClient.new.main
