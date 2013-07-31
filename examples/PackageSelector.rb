# encoding: utf-8

# Package Selector example
module Yast
  class PackageSelectorClient < Client
    def main
      Yast.import "UI"
      Yast.import "Pkg"
      # Pkg::SourceCreate( "http://dist.suse.de/install/SLP/SUSE-10.1-Beta7/i386/CD1/", "" );
      # Pkg::SourceCreate( "http://dist.suse.de/install/SLP/SUSE-10.0-RC4/i386/CD1/", "" );
      # Pkg::SourceCreate( "file:/srv/10.1-i386/CD1/", "" );
      Pkg.SourceCreate("file:/srv/10.1-i386/DVD1/", "")
      # Pkg::SourceCreate( "file:/srv/sles-10-i386/CD1/", "" );

      if true
        Pkg.TargetInit(
          "/", # installed system
          false
        ) # don't create a new RPM database
      end

      UI.OpenDialog(
        Opt(:defaultsize),
        PackageSelector(Id(:selector), "/dev/fd0")
      )
      @input = UI.RunPkgSelection(Id(:selector))
      UI.CloseDialog

      Builtins.y2milestone("Input: %1", @input)

      nil
    end
  end
end

Yast::PackageSelectorClient.new.main
