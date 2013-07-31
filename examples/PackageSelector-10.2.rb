# encoding: utf-8

# Package Selector example
module Yast
  class PackageSelector102Client < Client
    def main
      Yast.import "UI"
      Yast.import "Pkg"
      @use_installed_system = true

      Pkg.SourceCreate("file:/mounts/dist/full/full-10.2-i386/", "")


      Pkg.TargetInit("/", false) if @use_installed_system

      UI.OpenDialog(
        Opt(:defaultsize),
        PackageSelector(Id(:selector), "/dev/fd0")
      )
      @button = UI.RunPkgSelection(Id(:selector))
      UI.CloseDialog

      Builtins.y2milestone("Button: %1", @button)

      nil
    end
  end
end

Yast::PackageSelector102Client.new.main
