# encoding: utf-8

# Advanced Slider + BarGraph example:
#
# Windows partition splitter,
# this time with two sliders
module Yast
  class WinResizer2Client < Client
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
      @suggested_linux_size = 800
      @linux_size = @suggested_linux_size
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
            VSpacing(0.5),
            Slider(
              Id(:win_free),
              Opt(:notify),
              Ops.add(Ops.add("Windows free (", @unit), ")"),
              @min_free,
              Ops.subtract(@total_free, @linux_min),
              @win_free
            ),
            VSpacing(0.5),
            Slider(
              Id(:linux_size),
              Opt(:notify),
              Ops.add(Ops.add("Linux (", @unit), ")"),
              @linux_min,
              Ops.subtract(@total_free, @min_free),
              @linux_size
            ),
            VSpacing(0.5),
            HBox(
              HCenter(PushButton(Id(:suggest), "&Default")),
              PushButton(Id(:close), Opt(:default), "&Close")
            )
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
          UI.ChangeWidget(Id(:win_free), :Value, @win_free)
        elsif @widget == :win_free
          @win_free = Convert.to_integer(UI.QueryWidget(Id(:win_free), :Value))
          @linux_size = Ops.subtract(@total_free, @win_free)
          UI.ChangeWidget(
            Id(:graph),
            :Values,
            [@win_used, @win_free, @linux_size]
          )
          UI.ChangeWidget(Id(:linux_size), :Value, @linux_size)
        elsif @widget == :suggest
          @linux_size = @suggested_linux_size
          @win_free = Ops.subtract(@total_free, @linux_size)
          UI.ChangeWidget(
            Id(:graph),
            :Values,
            [@win_used, @win_free, @linux_size]
          )
          UI.ChangeWidget(Id(:linux_size), :Value, @linux_size)
          UI.ChangeWidget(Id(:win_free), :Value, @win_free)
        end
      end while @widget != :close && @widget != :cancel

      UI.CloseDialog

      nil
    end
  end
end

Yast::WinResizer2Client.new.main
