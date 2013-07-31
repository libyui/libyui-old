# encoding: utf-8

# Advanced example of using the Wizard widget.
#
# Note: YCP applications are discouraged from using the Wizard widget directly.
# Use the Wizard module instead.
module Yast
  class Wizard4Client < Client
    def main
      Yast.import "UI"
      if !UI.HasSpecialWidget(:Wizard)
        Builtins.y2error(
          "This works only with UIs that provide the wizard widget!"
        )
        return
      end

      @help_text = "<p>This is a help text.</p>" +
        "<p>It should be helpful.</p>" +
        "<p>If it isn't helpful, it should rather not be called a <i>help text</i>.</p>"

      UI.OpenDialog(
        Opt(:defaultsize),
        Wizard(
          Opt(:treeEnabled),
          :back,
          "&Back",
          :abort,
          "Ab&ort",
          :next,
          "&Next"
        )
      )

      # UI::DumpWidgetTree();


      UI.WizardCommand(
        term(
          :SetDialogIcon,
          "/usr/share/YaST2/theme/current/icons/22x22/apps/YaST.png"
        )
      )
      UI.WizardCommand(
        term(:SetDialogHeading, "Welcome to the YaST2 installation")
      )
      UI.WizardCommand(term(:SetHelpText, @help_text))


      UI.WizardCommand(term(:AddTreeItem, "", "First Toplevel Item", "tl1"))
      UI.WizardCommand(term(:AddTreeItem, "", "Second Toplevel Item", "tl2"))
      UI.WizardCommand(term(:AddTreeItem, "", "Third Toplevel Item", "tl3"))

      UI.WizardCommand(term(:AddTreeItem, "tl1", "First Sublevel", "1-1"))
      UI.WizardCommand(term(:AddTreeItem, "tl1", "Second Sublevel", "1-2"))
      UI.WizardCommand(term(:AddTreeItem, "tl1", "Third Sublevel", "1-3"))

      UI.WizardCommand(term(:AddTreeItem, "tl2", "First Sublevel", "2-1"))
      UI.WizardCommand(term(:AddTreeItem, "tl2", "Second Sublevel", "2-2"))
      UI.WizardCommand(term(:AddTreeItem, "tl2", "Third Sublevel", "2-3"))

      UI.WizardCommand(term(:AddTreeItem, "1-2", "First 3rd level ", "3rd 1"))
      UI.WizardCommand(term(:AddTreeItem, "1-2", "Second 3rd level ", "3rd 2"))
      UI.WizardCommand(term(:AddTreeItem, "1-2", "Item without ID", ""))

      UI.WizardCommand(term(:SelectTreeItem, "3rd 1"))



      UI.WizardCommand(term(:AddMenu, "&File", "file-menu"))
      UI.WizardCommand(term(:AddMenu, "&Edit", "edit-menu"))
      UI.WizardCommand(term(:AddMenu, "&Options", "opt-menu"))

      UI.WizardCommand(term(:AddMenuEntry, "file-menu", "&New", "file-new"))
      UI.WizardCommand(term(:AddMenuEntry, "file-menu", "&Open", "file-open"))
      UI.WizardCommand(
        term(:AddSubMenu, "file-menu", "Open &Recent", "file-recent")
      )
      UI.WizardCommand(term(:AddMenuEntry, "file-menu", "&Save", "file-save"))
      UI.WizardCommand(
        term(:AddMenuEntry, "file-menu", "Save &As", "file-save-as")
      )

      UI.WizardCommand(
        term(:AddMenuEntry, "file-recent", "/tmp/test1", "recent-test1")
      )
      UI.WizardCommand(
        term(:AddMenuEntry, "file-recent", "/tmp/test2", "recent-test2")
      )

      UI.WizardCommand(term(:AddMenuEntry, "edit-menu", "C&ut", "edit-cut"))
      UI.WizardCommand(term(:AddMenuEntry, "edit-menu", "C&opy", "edit-copy"))
      UI.WizardCommand(term(:AddMenuEntry, "edit-menu", "&Paste", "edit-paste"))

      UI.WizardCommand(
        term(:AddMenuEntry, "opt-menu", "&Settings", "opt-settings")
      )
      UI.WizardCommand(term(:AddMenuSeparator, "opt-menu"))
      UI.WizardCommand(
        term(
          :AddMenuEntry,
          "opt-menu",
          "Activate &Hypersonic Transducer",
          "frank-n-furter"
        )
      )



      while true
        @event = UI.WaitForEvent

        Builtins.y2milestone("Got event: %1", @event)

        break if Ops.get(@event, "ID") == :abort

        Builtins.y2milestone(
          "Tree selection: %1",
          UI.QueryWidget(Id(:wizard), :CurrentItem)
        )
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::Wizard4Client.new.main
