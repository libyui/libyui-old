# encoding: utf-8

# Simple example for PatternSelector
module Yast
  class PatternSelectorEmptyClient < Client
    def main
      Yast.import "UI"
      if !UI.HasSpecialWidget(:PatternSelector)
        UI.OpenDialog(
          VBox(
            Label("Error: This UI doesn't support the PatternSelector widget!"),
            PushButton(Opt(:default), "&OK")
          )
        )
        UI.UserInput
        UI.CloseDialog

        return
      end


      UI.OpenDialog(Opt(:defaultsize), PatternSelector(Id(:selector)))
      @input = UI.RunPkgSelection(Id(:selector))
      UI.CloseDialog

      Builtins.y2milestone("Input: %1", @input)

      nil
    end
  end
end

Yast::PatternSelectorEmptyClient.new.main
