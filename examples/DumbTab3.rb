# encoding: utf-8

# Minimalistic example for tab widget
module Yast
  class DumbTab3Client < Client
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
            Id(:tab),
            ["Page 1", "Page 2", "Page 3"],
            RichText(Id(:contents), "Contents")
          ),
          Left(
            HBox(
              PushButton(Id(:page1), "Page &1"),
              PushButton(Id(:page2), "Page &2"),
              PushButton(Id(:page3), "Page &3")
            )
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

        if @input == :page1
          UI.ChangeWidget(:tab, :CurrentItem, "Page 1")
        elsif @input == :page2
          UI.ChangeWidget(:tab, :CurrentItem, "Page 2")
        elsif @input == :page3
          UI.ChangeWidget(:tab, :CurrentItem, "Page 3")
        end

        Builtins.y2milestone(
          "Current tab: %1",
          UI.QueryWidget(:tab, :CurrentItem)
        )
      end until @input == :close


      UI.CloseDialog

      nil
    end
  end
end

Yast::DumbTab3Client.new.main
