# encoding: utf-8

module Yast
  class Enabling3Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          VBox(
            Id(:box),
            Opt(:disabled),
            InputField("TestEntry", ""),
            CheckBox("Looks fine")
          ),
          PushButton(Id(:change), "&Change"),
          PushButton(Id(:cancel), "&Quit")
        )
      )

      @enabled = false

      while UI.UserInput != :cancel
        @enabled = !@enabled
        UI.ChangeWidget(Id(:box), :Enabled, @enabled)
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::Enabling3Client.new.main
