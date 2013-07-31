# encoding: utf-8

module Yast
  class ContextMenuClient < Client
    def main
      Yast.import "UI"


      @event_display = "<i>No event yet - click something (or wait for timeout)</i>"


      if !UI.HasSpecialWidget(:Slider)
        UI.OpenDialog(
          VBox(
            Label("ContextMenu not supported!"),
            PushButton(Opt(:default), "&Oops!")
          )
        )
        UI.UserInput
        UI.CloseDialog
        return
      end





      UI.OpenDialog(
        VBox(
          SelectionBox(
            Id(:sport),
            Opt(:notify, :immediate, :notifyContextMenu),
            "Select your sport:",
            ["Swimming", "Cycling", "Running"]
          ),
          RichText(Id(:event_display), @event_display),
          PushButton(Id(:close), "&OK")
        )
      )

      #
      # Event loop
      #

      @event = {}
      begin
        @event = UI.WaitForEvent(100000)
        if @event != nil
          UI.ChangeWidget(:event_display, :Value, formatEvent(@event))
        end

        if @event != nil &&
            Ops.get_string(@event, "EventReason", "") == "ContextMenuActivated"
          # UI::OpenContextMenu( `menu(
          # 				    [	`item(`id(`folder), "&Folder"),
          # 					`item(`id(`text),   "&Text File" ),
          # 					`item(`id(`image),  "&Image"     )
          # 				    ]));


          UI.OpenContextMenu(
            term(
              :menu,
              [
                Item(Id(:instruct), "Instruc&tions"),
                term(
                  :menu,
                  "&Execute",
                  [Item(Id(:training), "&Training"), Item(Id(:race), "&Race")]
                )
              ]
            )
          )
        end
      end until Ops.get(@event, "ID") == :close


      UI.CloseDialog

      nil
    end

    def formatEvent(event)
      event = deep_copy(event)
      html = "<h3>Event:</h3><p>"
      Builtins.y2milestone("formatEvent(%1)", event)
      Builtins.foreach(event) do |key, value|
        html = Ops.add(
          Ops.add(
            Ops.add(
              Ops.add(Ops.add(html, "<font color=blue>"), key),
              "</font>: "
            ),
            Builtins.tostring(value)
          ),
          "<br>"
        )
      end
      html = Ops.add(html, "</p>")
      html
    end
  end
end

Yast::ContextMenuClient.new.main
