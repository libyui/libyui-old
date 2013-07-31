# encoding: utf-8

# SetDesktopTitle Example
#
# Set the window title that is shown by the window manager
# Searches for the desktop file and uses its name attribute
# Note: this works in qt and gtk only, ncurses doesn't have a window title
module Yast
  class WizardSetDesktopTitleClient < Client
    def main
      Yast.import "Wizard"
      Yast.import "Label"

      Wizard.CreateDialog
      Wizard.SetContentsButtons(
        "SetDesktopTitle Example",
        Label(
          "Read 'Network Settings' from the lan.desktop file and set this string as window title"
        ),
        "Help",
        Label.BackButton,
        Label.NextButton
      )

      Wizard.SetDesktopTitle("lan") # use "lan" as an example

      Wizard.UserInput
      Wizard.CloseDialog

      nil
    end
  end
end

Yast::WizardSetDesktopTitleClient.new.main
