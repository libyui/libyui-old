# encoding: utf-8

module Yast
  class Enabling2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          InputField(Id(:test), Opt(:disabled), "TestEntry", "Disabled"),
          PushButton(Id(:change), Opt(:hstretch), "&Change")
        )
      )

      @enabled = false

      while UI.UserInput != :cancel
        @enabled = !@enabled
        UI.ChangeWidget(Id(:test), :Enabled, @enabled)

        if @enabled
          UI.ChangeWidget(Id(:test), :Value, "Enabled")
        else
          UI.ChangeWidget(Id(:test), :Value, "Disabled")
        end
      end

      nil
    end
  end
end

Yast::Enabling2Client.new.main
