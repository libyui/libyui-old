# encoding: utf-8

# Advanced Slider + BarGraph example:
#
# Display a dialog with a bar graph for RGB color percentages
# and 3 sliders for the RGB percentage.
# Update the bar graph while the user adjusts the RGB values.
#
# Unfortunately the colors don't match any more in the BarGraph widget - they
# used to be red, blue and green. You need to use a bit of imagination
# here. ;-)
module Yast
  class Slider2Client < Client
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


      # Initialize RGB values

      @red = 128
      @blue = 128
      @green = 128


      # Create the dialog

      UI.OpenDialog(
        VBox(
          HSpacing(50), # force width
          BarGraph(
            Id(:graph),
            [@red, @green, @blue],
            ["Red\n%1", "Green\n%1", "Blue\n%1"]
          ),
          Slider(Id(:red), Opt(:notify), "Red", 0, 255, @red),
          Slider(Id(:green), Opt(:notify), "Green", 0, 255, @green),
          Slider(Id(:blue), Opt(:notify), "Blue", 0, 255, @blue),
          PushButton(Id(:close), Opt(:default), "&Close")
        )
      )


      # Event processing loop - left only via the "close" button
      # or the window manager close button / function.
      @widget = nil
      begin
        @widget = UI.UserInput

        if @widget == :red || @widget == :blue || # any of the sliders?
            @widget == :green
          # Get all slider values

          @red = Convert.to_integer(UI.QueryWidget(Id(:red), :Value))
          @green = Convert.to_integer(UI.QueryWidget(Id(:green), :Value))
          @blue = Convert.to_integer(UI.QueryWidget(Id(:blue), :Value))


          # Update bar graph

          UI.ChangeWidget(Id(:graph), :Values, [@red, @green, @blue])
        end
      end while @widget != :close && @widget != :cancel # the real "Close" button # the window manager close function/button

      UI.CloseDialog

      nil
    end
  end
end

Yast::Slider2Client.new.main
