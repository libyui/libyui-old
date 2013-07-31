# encoding: utf-8

module Yast
  class PackageSelectorEmptyClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        PackageSelector(Id(:selector), Opt(:testMode))
      )
      @input = UI.RunPkgSelection(Id(:selector))
      UI.CloseDialog

      Builtins.y2milestone("Input: %1", @input)

      nil
    end
  end
end

Yast::PackageSelectorEmptyClient.new.main
