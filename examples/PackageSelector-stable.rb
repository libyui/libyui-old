# encoding: utf-8

# Package Selector example
module Yast
  class PackageSelectorStableClient < Client
    def main
      Yast.import "UI"
      Yast.import "Pkg"
      Pkg.SourceCreate("file:/mounts/machcd/CDs/openSUSE-10.2-RC5-FTP-OSS", "")
      @useInstalledSystem = true

      if @useInstalledSystem
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

Yast::PackageSelectorStableClient.new.main
