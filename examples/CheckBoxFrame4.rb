# encoding: utf-8

module Yast
  class CheckBoxFrame4Client < Client
    def main
      Yast.import "UI"

      @frame = VBox(
        CheckBoxFrame(
          Id(:foo),
          Opt(:notify),
          "Selection A",
          true,
          VBox(
            CheckBox("Enable Foo"),
            HBox(
              HSpacing(2),
              Left(
                CheckBoxFrame(
                  Id(:bar),
                  "Selection B",
                  false,
                  VBox(
                    CheckBox("Enable Bar"),
                    SelectionBox(Id(:sel), "Select", ["Foo", "Bar", "Baz"]),
                    CheckBoxFrame(
                      Id(:c),
                      "Selection C",
                      true,
                      CheckBox("Another")
                    )
                  )
                )
              )
            ),
            CheckBox("Disable Blubber")
          )
        ),
        PushButton(Id(:ok), "OK")
      )

      UI.OpenDialog(@frame)

      @id = nil
      begin
        @id = UI.UserInput
      end until @id == :ok

      UI.CloseDialog

      nil
    end
  end
end

Yast::CheckBoxFrame4Client.new.main
