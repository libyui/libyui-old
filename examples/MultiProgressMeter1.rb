# encoding: utf-8

# Simple example for MultiProgressMeter
module Yast
  class MultiProgressMeter1Client < Client
    def main
      Yast.import "UI"
      if !UI.HasSpecialWidget(:HMultiProgressMeter)
        UI.OpenDialog(
          VBox(
            Label(
              "Error: This UI doesn't support the MultiProgressMeter widget!"
            ),
            PushButton(Opt(:default), "&OK")
          )
        )
        UI.UserInput
        UI.CloseDialog

        return
      end


      UI.OpenDialog(
        VBox(
          VMultiProgressMeter(Id(:prog), [1000, 200, 500, 20, 100]),
          PushButton(Opt(:default), "&Ok")
        )
      )

      UI.UserInput

      UI.ChangeWidget(:prog, :Values, [100, 0, 0, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [200, 0, 0, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [300, 0, 0, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [400, 0, 0, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [500, 0, 0, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [600, 0, 0, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [700, 0, 0, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [800, 0, 0, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [900, 0, 0, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [1000, 0, 0, 0, 0])
      UI.UserInput

      UI.ChangeWidget(:prog, :Values, [1000, 50, 0, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [1000, 100, 0, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [1000, 150, 0, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [1000, 200, 0, 0, 0])
      UI.UserInput

      UI.ChangeWidget(:prog, :Values, [1000, 200, 100, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [1000, 200, 200, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [1000, 200, 300, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [1000, 200, 400, 0, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [1000, 200, 500, 0, 0])
      UI.UserInput

      UI.ChangeWidget(:prog, :Values, [1000, 200, 500, 10, 0])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [1000, 200, 500, 20, 0])
      UI.UserInput

      UI.ChangeWidget(:prog, :Values, [1000, 200, 500, 20, 20])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [1000, 200, 500, 20, 40])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [1000, 200, 500, 20, 80])
      UI.UserInput
      UI.ChangeWidget(:prog, :Values, [1000, 200, 500, 20, 100])
      UI.UserInput

      UI.UserInput

      UI.CloseDialog

      nil
    end
  end
end

Yast::MultiProgressMeter1Client.new.main
