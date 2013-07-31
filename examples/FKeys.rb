# encoding: utf-8

module Yast
  class FKeysClient < Client
    def main
      Yast.import "UI"
      # Set default function keys.
      #
      # Those examples are taken from the YaST2 style guide.
      # Real applications should of course use the predefined
      # button labels from the Label module.

      # define void UI::SetFunctionKeys( map fkeys ) ``{};

      UI.SetFunctionKeys(
        {
          "Help"             => 1,
          "Info"             => 2,
          "Show Details"     => 2,
          "Add"              => 3,
          "Edit"             => 4,
          "Delete"           => 5,
          "Test"             => 6,
          "Expert..."        => 7,
          "Advanced..."      => 7,
          "Back"             => 8,
          "Abort"            => 9,
          "Cancel"           => 9,
          "No"               => 9,
          "Next"             => 10,
          "OK"               => 10,
          "Accept"           => 10,
          "Finish"           => 10,
          "Yes"              => 10,
          "Apply"            => 10,
          "Undo"             => 11,
          "Reset"            => 11,
          "Defaults"         => 12,
          "Restore Defaults" => 12
        }
      )

      @help = "Use this with the NCurses UI.\n" +
        "Press [F1] there to show function key bindings."

      UI.OpenDialog(
        VBox(
          VSpacing(),
          Label(@help),
          VSpacing(2),
          HBox(
            Label("Last Button: "),
            Label(Id(:feedback), Opt(:outputField, :hstretch), "")
          ),
          VSpacing(2),
          HBox(
            HWeight(1, PushButton(Id("Add"), "&Add")),
            HWeight(1, PushButton(Id("Edit"), "&Edit")),
            HWeight(1, PushButton(Id("Delete"), "&Delete")),
            HWeight(1, Empty()),
            HWeight(1, PushButton(Id("Info"), "&Info"))
          ),
          VSpacing(),
          HBox(
            HWeight(1, PushButton(Id("Back"), "&Back")),
            HWeight(1, Empty()),
            HWeight(1, PushButton(Id("Abort"), "Ab&ort")),
            HWeight(1, Empty()),
            HWeight(1, PushButton(Id("Next"), "&Next"))
          ),
          VSpacing(3),
          Right(PushButton(Id("Close"), "&Close"))
        )
      )

      # Using string widget IDs for the buttons so they can easily be printed
      # into the feedback field.
      @button = ""
      begin
        @button = UI.UserInput
        Builtins.y2milestone("Input: %1", @button)
        UI.ChangeWidget(Id(:feedback), :Value, @button)
      end while @button != "Close"

      UI.CloseDialog

      nil
    end
  end
end

Yast::FKeysClient.new.main
