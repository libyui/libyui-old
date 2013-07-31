# encoding: utf-8

# YOU mode
module Yast
  class PackageSelectorYOUClient < Client
    def main
      Yast.import "UI"
      Yast.import "Pkg"
      Pkg.SourceCreate(
        "http://armstrong.suse.de/download/Code/10/update/i386.ro/",
        "/"
      )

      if true
        Pkg.TargetInit(
          "/", # installed system
          false
        ) # don't create a new RPM database
      end

      UI.OpenDialog(
        Opt(:defaultsize),
        PackageSelector(Id(:selector), Opt(:testMode, :youMode))
      )
      #				    `opt(`testMode ) ) );
      UI.RunPkgSelection(Id(:selector))
      UI.CloseDialog

      nil
    end
  end
end

Yast::PackageSelectorYOUClient.new.main
