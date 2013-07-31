# encoding: utf-8

module Yast
  class InputField4Client < Client
    def main
      Yast.import "UI"
      # Build dialog with one input field field, 4 Beatles buttons and an OK button.
      UI.OpenDialog(
        VBox(
          InputField(Id(:name), "Name:"),
          HBox(
            PushButton(Id(:john), "&John"),
            PushButton(Id(:paul), "&Paul"),
            PushButton(Id(:george), "&George"),
            PushButton(Id(:ringo), "&Ringo")
          ),
          PushButton(Id(:ok), "&OK")
        )
      )

      # Wait for user input.
      @button = nil
      begin
        @button = UI.UserInput

        if @button == :john
          UI.ChangeWidget(Id(:name), :Value, "John Lennon")
        elsif @button == :paul
          UI.ChangeWidget(Id(:name), :Value, "Paul McCartney")
        elsif @button == :george
          UI.ChangeWidget(Id(:name), :Value, "George Harrison")
        elsif @button == :ringo
          UI.ChangeWidget(Id(:name), :Value, "Ringo Starr")
        end
      end until @button == :ok

      UI.CloseDialog

      nil
    end
  end
end

Yast::InputField4Client.new.main
