# encoding: utf-8

# Popup stress test: Pop up dialogs and close them all the time
module Yast
  class PopupStressTestClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          Heading("Pop-up Stress Test"),
          VSpacing(5),
          Label(
            "Sit back and watch the pop-up dialogs.\n" + "\n" +
              "Experiment with minimizing this application\n" +
              "and with working with some other application\n" +
              "while this is running."
          ),
          Bottom(Right(PushButton(Id(:cancel), Opt(:default), "&Close")))
        )
      )

      while true
        break if UI.TimeoutUserInput(3 * 1000) == :cancel

        UI.OpenDialog(
          VBox(
            Heading("Annoying pop-up #1"),
            PushButton(Id(:cancel), Opt(:default), "&Cancel")
          )
        )

        if UI.TimeoutUserInput(3 * 1000) == :cancel
          UI.CloseDialog
          break
        end

        UI.OpenDialog(
          VBox(
            Heading("Annoying pop-up #2"),
            PushButton(Id(:cancel), Opt(:default), "&Cancel")
          )
        )

        if UI.TimeoutUserInput(3 * 1000) == :cancel
          UI.CloseDialog
          UI.CloseDialog
          break
        end

        UI.CloseDialog # close pop-up #2

        if UI.TimeoutUserInput(3 * 1000) == :cancel
          UI.CloseDialog
          break
        end

        UI.CloseDialog # close pop-up #1
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::PopupStressTestClient.new.main
