# encoding: utf-8

# Advanced Slider + BarGraph example:
#
# Windows partition splitter.
module Yast
  class WinResizerClient < Client
    def main
      Yast.import "UI"
      # Check for availability of required widgets

      if !UI.HasSpecialWidget(:Slider) || !UI.HasSpecialWidget(:BarGraph)
        UI.OpenDialog(
          VBox(
            Label(
              "Error: This UI doesn't support the required special widgets!"
            ),
            PushButton(Opt(:default), "&OK")
          )
        )
        UI.UserInput
        UI.CloseDialog

        return
      end


      # Initialize values

      @unit = "MB"
      @win_used = 350
      @total_free = 1500
      @min_free = 50
      @linux_min = 300
      @linux_size = 800
      @win_free = Ops.subtract(@total_free, @linux_size)


      # Create the dialog

      UI.OpenDialog(
        VSquash(
          VBox(
            HSpacing(50), # force width
            Left(Label("Now:")),
            BarGraph(
              Opt(:vstretch),
              [@win_used, @total_free],
              [
                Ops.add("Windows\nused\n%1 ", @unit),
                Ops.add("Windows\nfree\n%1 ", @unit)
              ]
            ),
            VSpacing(2),
            Left(Label("After installation:")),
            BarGraph(
              Id(:graph),
              Opt(:vstretch),
              [@win_used, @win_free, @linux_size],
              [
                Ops.add("Windows\nused\n%1 ", @unit),
                Ops.add("Windows\nfree\n%1 ", @unit),
                Ops.add("Linux\n%1 ", @unit)
              ]
            ),
            Slider(
              Id(:linux_size),
              Opt(:notify),
              Ops.add(Ops.add("Linux (", @unit), ")"),
              @linux_min,
              Ops.subtract(@total_free, @min_free),
              @linux_size
            ),
            PushButton(Id(:close), Opt(:default), "&Close")
          )
        )
      )

      # Event processing loop - left only via the "close" button
      # or the window manager close button / function.

      @widget = nil
      begin
        @widget = UI.UserInput

        if @widget == :linux_size
          @linux_size = Convert.to_integer(
            UI.QueryWidget(Id(:linux_size), :Value)
          )
          @win_free = Ops.subtract(@total_free, @linux_size)
          UI.ChangeWidget(
            Id(:graph),
            :Values,
            [@win_used, @win_free, @linux_size]
          )
        end
      end while @widget != :close && @widget != :cancel

      UI.CloseDialog

      nil
    end
  end
end

Yast::WinResizerClient.new.main
