# encoding: utf-8

module Yast
  class Spacing1Client < Client
    def main
      Yast.import "UI"
      # Build dialog with one input field field, 4 Beatles buttons and an OK button.
      UI.OpenDialog(
        VBox(
          VSpacing(),
          HBox(Label("Name:"), InputField(Id(:name), "")),
          VSpacing(0.2),
          HBox(
            PushButton(Id(:john), "&John"),
            HSpacing(0.5),
            PushButton(Id(:paul), "&Paul"),
            HSpacing(3),
            PushButton(Id(:george), "&George"),
            HSpacing(0.5),
            PushButton(Id(:ringo), "&Ringo")
          ),
          VSpacing(0.5),
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

Yast::Spacing1Client.new.main
