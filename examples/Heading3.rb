# encoding: utf-8

module Yast
  class Heading3Client < Client
    def main
      Yast.import "UI"
      # Build dialog with one label, 4 Beatles buttons and an OK button.
      UI.OpenDialog(
        VBox(
          Label("My favourite Beatle:"),
          # `Heading(`id(`favourite), "Press one of the buttons below"),
          Heading(Id(:favourite), "(please select one)"),
          HBox(
            PushButton(Id(:john), "John"),
            PushButton(Id(:paul), "Paul"),
            PushButton(Id(:george), "George"),
            PushButton(Id(:ringo), "Ringo")
          ),
          PushButton(Id(:ok), "&OK")
        )
      )

      # Wait for user input.
      @button = nil
      begin
        @button = UI.UserInput

        if @button == :john
          UI.ChangeWidget(Id(:favourite), :Value, "John Lennon")
        elsif @button == :paul
          UI.ChangeWidget(Id(:favourite), :Value, "Paul McCartney")
        elsif @button == :george
          UI.ChangeWidget(Id(:favourite), :Value, "George Harrison")
        elsif @button == :ringo
          UI.ChangeWidget(Id(:favourite), :Value, "Ringo Starr")
        end
      end until @button == :ok

      nil
    end
  end
end

Yast::Heading3Client.new.main
