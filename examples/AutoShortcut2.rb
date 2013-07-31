# encoding: utf-8

module Yast
  class AutoShortcut2Client < Client
    def main
      Yast.import "UI"
      # Demo for automatically generated shortcuts.
      #
      # This is a more realistic example - it points out how the `autoShortcut
      # option is intended to be used. See 'AutoShortcut1.ycp' for a simpler example.
      #
      # There shouldn't be any complaints about shortcuts in the log file when this is started.


      @sw_selections = [
        "Minimum System",
        "Minimum X11 System",
        "Gnome System",
        "Default (KDE)",
        "Office System (KDE Based)",
        "Almost Everything"
      ]

      @radio_box = VBox()

      Builtins.foreach(@sw_selections) do |sel|
        @radio_box = Builtins.add(
          @radio_box,
          Left(RadioButton(Opt(:autoShortcut), sel))
        )
      end

      Builtins.y2milestone("radio_box: %1", @radio_box)

      UI.OpenDialog(
        VBox(
          RadioButtonGroup(Frame("Software Selection", HVSquash(@radio_box))),
          PushButton("&OK")
        )
      )

      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::AutoShortcut2Client.new.main
