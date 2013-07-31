# encoding: utf-8

# Advanced BarGraph example:
#
# Create a dialog with a BarGraph with a number of segments
# and a "+" and a "-" button for each segment.
module Yast
  class BarGraph3Client < Client
    def main
      Yast.import "UI"
      # Check for availability of the BarGraph widget - this is necessary since
      # this is an optional widget that not all UIs need to support.

      if !UI.HasSpecialWidget(:BarGraph)
        # Pop up error message if the BarGraph widget is not available

        UI.OpenDialog(
          VBox(
            Label("Error: This UI doesn't support the BarGraph widget!"),
            PushButton(Opt(:default), "&OK")
          )
        )
        UI.UserInput
        UI.CloseDialog

        return
      end



      # list	values = [ 100, 200, 300, 150, 250, 120, 200, 120 ];
      @values = [100, 100, 100, 100, 100, 100, 100, 100]
      @inc = 10 # increment / decrement for each button press

      # Create the main dialog:
      #
      # One BarGraph at the top, below that two rows of equal sized (thus the
      # weights) buttons, below that a "close" button.
      #
      # The "+" / "-" -buttons use an integer value as their ID which can be
      # used to point to the index of the value to be changed. If the ID is
      # negative it means subtract rather than add.

      @plus_buttons = HBox()
      @minus_buttons = HBox()
      @i = 1

      Builtins.foreach(@values) do |val|
        @plus_buttons = Builtins.add(
          @plus_buttons,
          HWeight(1, PushButton(Id(@i), "+"))
        )
        @minus_buttons = Builtins.add(
          @minus_buttons,
          HWeight(1, PushButton(Id(Ops.unary_minus(@i)), "-"))
        )
        @i = Ops.add(@i, 1)
      end

      UI.OpenDialog(
        VBox(
          BarGraph(Id(:bar), @values),
          @plus_buttons,
          @minus_buttons,
          PushButton(Id(:close), Opt(:default), "&Close")
        )
      )


      # Event processing loop - left only via the "close" button
      # or the window manager close button / function.

      @button_id = nil
      begin
        @button_id = UI.UserInput # wait for button click

        if @button_id != :close && @button_id != :cancel
          @sign = 1

          if Ops.less_than(@button_id, 0)
            @sign = -1
            @button_id = Ops.unary_minus(Convert.to_integer(@button_id))
          end

          # Loop over the values. Increment the value corresponding to the
          # clicked button, decrement all others as to maintain the total
          # sum of all values - or vice versa for negative button IDs
          # (i.e. "-" buttons).

          @new_values = []
          @i2 = 0

          while Ops.less_than(@i2, Builtins.size(@values))
            @old_val = Ops.get(@values, @i2, 0)

            if Ops.add(@i2, 1) == @button_id
              @new_values = Builtins.add(
                @new_values,
                Ops.add(@old_val, Ops.multiply(@sign, @inc))
              )
            else
              @new_values = Builtins.add(
                @new_values,
                Ops.add(
                  @old_val,
                  Ops.multiply(
                    Ops.unary_minus(@sign),
                    Ops.divide(@inc, Ops.subtract(Builtins.size(@values), 1))
                  )
                )
              )
            end

            @i2 = Ops.add(@i2, 1)
          end

          @values = deep_copy(@new_values)
          UI.ChangeWidget(Id(:bar), :Values, @values)
        end
      end while @button_id != :close && @button_id != :cancel

      UI.CloseDialog

      nil
    end
  end
end

Yast::BarGraph3Client.new.main
