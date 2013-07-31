# encoding: utf-8

# Simple demo for the UI::GetDisplayInfo() UI bultin:
#
# Open a RichText widget with all the info map's contents and format them as HTML
#
module Yast
  class GetDisplayInfoClient < Client
    def main
      Yast.import "UI"
      @info_map = UI.GetDisplayInfo
      @info_text = "<ul>"

      Builtins.foreach(@info_map) do |capability, value|
        @info_text = Ops.add(
          @info_text,
          Builtins.sformat("<li>%1: %2</li>", capability, value)
        )
      end

      @info_text = Ops.add(@info_text, "<ul>")

      UI.OpenDialog(
        VBox(
          HSpacing(40), # force width
          Left(Heading("Display capabilities:")),
          HBox(
            RichText(@info_text), # force height
            VSpacing(15)
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

Yast::GetDisplayInfoClient.new.main
