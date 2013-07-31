# encoding: utf-8

# Simple example for PatternSelector
module Yast
  class PatternSelectorSoloClient < Client
    def main
      Yast.import "Pkg"
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


      Pkg.TargetInit(
        "/", # installed system
        false
      ) # don't create a new RPM database

      UI.OpenDialog(Opt(:defaultsize), PatternSelector(Id(:selector)))
      @input = UI.RunPkgSelection(Id(:selector))
      UI.CloseDialog

      Builtins.y2milestone("Input: %1", @input)

      nil
    end
  end
end

Yast::PatternSelectorSoloClient.new.main
