# encoding: utf-8

# Package Selector example
module Yast
  class PackageSelectorMultiSrcClient < Client
    def main
      Yast.import "UI"
      Yast.import "Pkg"
      Pkg.SourceCreate("file:/srv/sles-10-i386/CD1/", "")
      Pkg.SourceCreate("file:/srv/10.1-i386/CD1/", "")
      # Pkg::SourceCreate( "http://ftp.uni-erlangen.de/pub/mirrors/packman/suse/10.0/", "" );
      # Pkg::SourceCreate( "ftp://ftp.gwdg.de/pub/linux/misc/packman/suse/10.0/", "" );
      Pkg.TargetInit("/", false) # installed system

      UI.OpenDialog(
        Opt(:defaultsize),
        PackageSelector(
          Id(:selector),
          Opt(:repoMode),
          # `opt(`testMode),
          "/dev/fd0"
        )
      )
      @input = UI.RunPkgSelection(Id(:selector))
      UI.CloseDialog

      Builtins.y2milestone("Input: %1", @input)

      nil
    end
  end
end

Yast::PackageSelectorMultiSrcClient.new.main
