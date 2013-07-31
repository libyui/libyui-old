# encoding: utf-8

# Package Selector example
module Yast
  class PackageSelector101StableClient < Client
    def main
      Yast.import "UI"
      Yast.import "Pkg"
      Pkg.SourceCreate(
        "file:/mounts/machcd2/CDs/SUSE-Linux-10.1-RC5-DVD-Retail-i386/DVD1",
        ""
      )


      if false
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

Yast::PackageSelector101StableClient.new.main
