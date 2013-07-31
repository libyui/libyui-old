# encoding: utf-8

module Yast
  class CheckBox1Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(CheckBox("A &checked check box\nwith multi-line", true))
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::CheckBox1Client.new.main
