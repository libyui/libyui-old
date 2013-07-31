# encoding: utf-8

# Simple example for SimplePatchSelector
module Yast
  class SimplePatchSelectorEmptyClient < Client
    def main
      Yast.import "UI"
      if !UI.HasSpecialWidget(:SimplePatchSelector)
        UI.OpenDialog(
          VBox(
            Label(
              "Error: This UI doesn't support the SimplePatchSelector widget!"
            ),
            PushButton(Opt(:default), "&OK")
          )
        )
        UI.UserInput
        UI.CloseDialog

        return
      end


      UI.OpenDialog(
        Opt(:defaultsize),
        term(:SimplePatchSelector, Id(:selector))
      )
      @input = UI.RunPkgSelection(Id(:selector))
      UI.CloseDialog

      Builtins.y2milestone("Input: %1", @input)

      nil
    end
  end
end

Yast::SimplePatchSelectorEmptyClient.new.main
