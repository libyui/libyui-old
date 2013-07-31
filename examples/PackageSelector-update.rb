# encoding: utf-8

module Yast
  class PackageSelectorUpdateClient < Client
    def main
      Yast.import "UI"
      Yast.import "Pkg"
      Pkg.TargetInit(
        "/", # installed system
        false
      ) # don't create a new RPM database
      UI.OpenDialog(
        Opt(:defaultsize),
        PackageSelector(Id(:selector), Opt(:testMode, :updateMode))
      )
      @input = UI.RunPkgSelection(Id(:selector))
      UI.CloseDialog

      Builtins.y2milestone("Input: %1", @input)

      nil
    end
  end
end

Yast::PackageSelectorUpdateClient.new.main
