# encoding: utf-8

module Yast
  class PartitionSplitter1Client < Client
    def main
      Yast.import "UI"
      if !UI.HasSpecialWidget(:PartitionSplitter)
        UI.OpenDialog(
          VBox(
            Label(
              "Error: This UI doesn't support the PartitionSplitter widget!"
            ),
            PushButton(Opt(:default), "&OK")
          )
        )
        UI.UserInput
        UI.CloseDialog

        return
      end

      @unit = "MB"
      @win_used = 350
      @total_free = 1500
      @min_free = 50
      @linux_min = 300
      @linux_size = 800

      UI.OpenDialog(
        VBox(
          HSpacing(60), # wider default size
          PartitionSplitter(
            @win_used,
            @total_free,
            @linux_size,
            @linux_min,
            @min_free,
            Ops.add("Windows\nused\n%1 ", @unit),
            Ops.add("Windows\nfree\n%1 ", @unit),
            Ops.add("Linux\n%1 ", @unit),
            Ops.add(Ops.add("Windows free (", @unit), ")"),
            Ops.add(Ops.add("Linux (", @unit), ")")
          ),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::PartitionSplitter1Client.new.main
