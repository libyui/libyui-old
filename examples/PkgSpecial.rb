# encoding: utf-8

# PkgSpecial example - not for general use!
#
module Yast
  class PkgSpecialClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Label("Hello, world!"),
          PkgSpecial("dummy1"),
          PkgSpecial("dummy2"),
          PkgSpecial("dummy3"),
          PushButton(Opt(:default), "&Ok")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::PkgSpecialClient.new.main
