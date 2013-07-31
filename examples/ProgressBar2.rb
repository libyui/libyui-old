# encoding: utf-8

# ProgressBar example
module Yast
  class ProgressBar2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Heading("Adjust the volume"),
          ProgressBar(Id(:vol), "Volume", 100, 50),
          HBox(
            PushButton(Id(:down), "<<"),
            PushButton(Id(:up), ">>"),
            HStretch(),
            HSpacing(3),
            PushButton(Id(:cancel), "&Close")
          )
        )
      )

      while true
        @button = Convert.to_symbol(UI.UserInput)

        if @button == :cancel
          break
        elsif @button == :down || @button == :up
          @volume = Convert.to_integer(UI.QueryWidget(Id(:vol), :Value))

          @volume = Ops.subtract(@volume, 5) if @button == :down
          @volume = Ops.add(@volume, 5) if @button == :up

          Builtins.y2milestone("Volume: %1", @volume)
          UI.ChangeWidget(Id(:vol), :Value, @volume)
        end
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::ProgressBar2Client.new.main
