# encoding: utf-8

module Yast
  class IntField3Client < Client
    def main
      Yast.import "UI"
      # `IntField in combination with `HWeight
      # (test case for ncurses bug #488757)
      UI.OpenDialog(
        HBox(
          HWeight(1, IntField("short", 0, 10000, 50)),
          HWeight(1, PushButton(Opt(:default), "long label"))
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::IntField3Client.new.main
