# encoding: utf-8

# Full-fledged simple patch selection
module Yast
  class SimplePatchSelectorStableClient < Client
    def main
      Yast.import "UI"
      Yast.import "Pkg"
      textdomain "bogus"

      # Initialize RPM DB as pkg src
      Pkg.TargetInit(
        "/", # installed system
        false
      ) # don't create a new RPM database

      # Pkg::SourceCreate( "file:/mounts/dist/install/stable-x86/", "" );
      Pkg.SourceCreate("ftp://ftp.gwdg.de/pub/suse/update/10.2", "")



      if !UI.HasSpecialWidget(:SimplePatchSelector)
        detailedSelection # Fallback: Do detailed selection right away
        return
      end


      UI.OpenDialog(
        Opt(:defaultsize),
        term(:SimplePatchSelector, Id(:selector))
      )

      @button = nil
      begin
        @button = Convert.to_symbol(UI.RunPkgSelection(Id(:selector)))
        Builtins.y2milestone(
          "SimplePatchSelector selector returned %1",
          @button
        )

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
      UI.ReplaceWidget(
        :rep,
        PackageSelector(Id(:packages), Opt(:youMode), "/dev/fd0")
      )

      input = Convert.to_symbol(UI.RunPkgSelection(Id(:packages)))
      Builtins.y2milestone("Package selector returned  %1", input)
      UI.CloseDialog

      nil
    end
  end
end

Yast::SimplePatchSelectorStableClient.new.main
