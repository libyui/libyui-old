# encoding: utf-8

module Yast
  class AutoShortcut1Client < Client
    def main
      Yast.import "UI"
      # (Minimalistic) Demo for automatically generated shortcuts.
      #
      # See 'AutoShortcut2.ycp' for a more realistic example.
      #
      # Please note this is _not_ how this option is meant to be used:
      # It is intended for automatically generated data, not for fixed widgets.
      # If you know your widget label at this point, manually add a keyboard
      # shortcut; this will almost always be much better than anything what can
      # be automatically generated.
      #
      #
      # There shouldn't be any complaints about shortcuts in the log file when this is started.

      UI.OpenDialog(
        VBox(
          RadioButtonGroup(
            Frame(
              "Software Selection",
              HVSquash(
                VBox(
                  Left(RadioButton(Opt(:autoShortcut), "Minimum System")),
                  Left(RadioButton(Opt(:autoShortcut), "Minimum X11 System")),
                  Left(RadioButton(Opt(:autoShortcut), "Gnome System")),
                  Left(RadioButton(Opt(:autoShortcut), "Default (KDE)")),
                  Left(RadioButton(Opt(:autoShortcut), "Default + Office")),
                  Left(RadioButton(Opt(:autoShortcut), "Almost Everything"))
                )
              )
            )
          ),
          PushButton("&OK")
        )
      )

      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::AutoShortcut1Client.new.main
