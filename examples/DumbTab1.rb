# encoding: utf-8

# Minimalistic example for tab widget
module Yast
  class DumbTab1Client < Client
    def main
      Yast.import "UI"
      if !UI.HasSpecialWidget(:DumbTab)
        UI.OpenDialog(
          VBox(
            Label("Error: This UI doesn't support the DumbTab widget!"),
            PushButton(Opt(:default), "&OK")
          )
        )
        UI.UserInput
        UI.CloseDialog

        return
      end


      UI.OpenDialog(
        VBox(
          DumbTab(
            ["Page 1", "Page 2", "Page 3"],
            RichText(Id(:contents), "Contents")
          ),
          Right(PushButton(Id(:close), "&Close"))
        )
      )

      UI.DumpWidgetTree

      @input = nil
      begin
        @input = UI.UserInput

        if Ops.is_string?(@input)
          UI.ChangeWidget(:contents, :Value, Convert.to_string(@input))
        end
      end until @input == :close


      UI.CloseDialog

      nil
    end
  end
end

Yast::DumbTab1Client.new.main
