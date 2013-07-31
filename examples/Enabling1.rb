# encoding: utf-8

module Yast
  class Enabling1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          PushButton(Id(:test), Opt(:hstretch, :disabled), "Disabled"),
          PushButton(Id(:change), Opt(:hstretch), "&Change")
        )
      )

      @enabled = false

      while UI.UserInput != :cancel
        @enabled = !@enabled
        UI.ChangeWidget(Id(:test), :Enabled, @enabled)

        if @enabled
          UI.ChangeWidget(Id(:test), :Label, "Enabled")
        else
          UI.ChangeWidget(Id(:test), :Label, "Disabled")
        end
      end

      nil
    end
  end
end

Yast::Enabling1Client.new.main
