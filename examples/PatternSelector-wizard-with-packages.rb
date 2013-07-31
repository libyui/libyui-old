# encoding: utf-8

# Full-fledged pattern selection
module Yast
  class PatternSelectorWizardWithPackagesClient < Client
    def main
      Yast.import "UI"
      Yast.import "Pkg"
      textdomain "bogus"

      # Pkg::SourceCreate( "http://dist.suse.de/install/SLP/SUSE-10.1-Beta3/i386/CD1/", "" );
      #    Pkg::SourceCreate( "file:/srv/sles-10-i386/CD1/", "" );
      Pkg.SourceStartManager(true)
      Pkg.TargetInit("/", false)



      if !UI.HasSpecialWidget(:PatternSelector)
        detailedSelection # Fallback: Do detailed selection right away
        return
      end


      UI.OpenDialog(
        Opt(:defaultsize),
        Wizard(:back, "", :cancel, "&Cancel", :ok, "&OK")
      )


      @help_text = _(
        "<p>The available software for this system is shown by category in the left column.  To view a description for an item, select it in the list.</p>"
      ) +
        _(
          "<p>Change the status of items by clicking on their status icon or right-click on any icon for a context menu. With the context menu you can also change the status of all items.</p>"
        ) +
        _(
          "<p><b>Details</b> opens the detailed software package selection where you can view and select individual software packages.</p>"
        ) +
        _(
          "<p>The <b>disk usage</b> display in the lower right corner shows the remaining disk space after all requested changes will have been performed. Please notice that hard disk partitions that are full or nearly full can degrade system performance and in some cases even cause serious problems. The system needs some available disk space to run properly.</p>"
        )

      UI.WizardCommand(
        term(
          :SetDialogIcon,
          "/usr/share/YaST2/theme/current/icons/22x22/apps/YaST.png"
        )
      )
      UI.WizardCommand(term(:SetDialogHeading, "Software Selection"))
      UI.WizardCommand(term(:SetHelpText, @help_text))

      Pkg.TargetInit(
        "/", # installed system
        false
      ) # don't create a new RPM database

      UI.ReplaceWidget(Id(:contents), PatternSelector(Id(:patterns)))


      @button = nil
      begin
        @button = Convert.to_symbol(UI.RunPkgSelection(Id(:patterns)))
        Builtins.y2milestone("Pattern selector returned %1", @button)

        detailedSelection if @button == :details
      end until @button == :cancel || @button == :accept

      UI.CloseDialog

      nil
    end

    def detailedSelection
      # Open empty dialog for instant feedback

      UI.OpenDialog(
        Opt(:defaultsize),
        ReplacePoint(Id(:rep), Label("Reading package database..."))
      )

      # This will take a while: Detailed package data are retrieved
      # while the package manager is initialized
      UI.ReplaceWidget(:rep, PackageSelector(Id(:packages), "/dev/fd0"))

      input = Convert.to_symbol(UI.RunPkgSelection(Id(:packages)))
      Builtins.y2milestone("Package selector returned  %1", input)
      UI.CloseDialog

      nil
    end
  end
end

Yast::PatternSelectorWizardWithPackagesClient.new.main
