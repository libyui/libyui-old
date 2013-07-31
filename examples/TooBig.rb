# encoding: utf-8

# Look into ~/.y2log. For widgets that don't get their
# minsizes, warnings appear.
module Yast
  class TooBigClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          PushButton(Id(1), "This"),
          PushButton(Id(2), "is some"),
          PushButton(Id(3), "very long label"),
          PushButton(Id(4), "that is supposed to"),
          PushButton(Id(5), "make this dialog to darn huge that"),
          PushButton(Id(6), "not all widgets get their minsize"),
          PushButton(Id(11), "This"),
          PushButton(Id(12), "is some"),
          PushButton(Id(13), "very long label"),
          PushButton(Id(14), "that is supposed to"),
          PushButton(Id(15), "make this dialog to darn huge that"),
          PushButton(Id(16), "not all widgets get their minsize"),
          PushButton(Id(21), "This"),
          PushButton(Id(22), "is some"),
          PushButton(Id(23), "very long label"),
          PushButton(Id(24), "that is supposed to"),
          PushButton(Id(25), "make this dialog to darn huge that"),
          PushButton(Id(26), "not all widgets get their minsize"),
          PushButton(Id(31), "This"),
          PushButton(Id(32), "is some"),
          PushButton(Id(33), "very long label"),
          PushButton(Id(34), "that is supposed to"),
          PushButton(Id(35), "make this dialog to darn huge that"),
          PushButton(Id(36), "not all widgets get their minsize"),
          PushButton(Id(41), "This"),
          PushButton(Id(42), "is some"),
          PushButton(Id(43), "very long label"),
          PushButton(Id(44), "that is supposed to"),
          PushButton(Id(45), "make this dialog to darn huge that"),
          PushButton(Id(46), "not all widgets get their minsize"),
          PushButton(Id(51), "This"),
          PushButton(Id(52), "is some"),
          PushButton(Id(53), "very long label"),
          PushButton(Id(54), "that is supposed to"),
          PushButton(Id(55), "make this dialog to darn huge that"),
          PushButton(Id(56), "not all widgets get their minsize")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::TooBigClient.new.main
