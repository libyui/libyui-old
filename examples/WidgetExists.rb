# encoding: utf-8

module Yast
  class WidgetExistsClient < Client
    def main
      Yast.import "UI"


      # main
      #

      UI.OpenDialog(
        VBox(
          ReplacePoint(Id(:rp_top), PushButton(Id(:top), "Top Button")),
          ReplacePoint(Id(:rp_center), PushButton(Id(:center), "Center Button")),
          ReplacePoint(Id(:rp_bottom), PushButton(Id(:bottom), "Bottom Button")),
          VSpacing(1),
          Label(Id(:summary), ""),
          VSpacing(1),
          PushButton(Id(:close), "&Close")
        )
      )

      # Better wait until now before doing the summary - it isn't much use before UI::OpenDialog()
      # since of course all UI::WidgetExists() calls return false until then.
      UI.ChangeWidget(Id(:summary), :Value, Summary())
      UI.RecalcLayout

      @button = nil
      begin
        @button = UI.UserInput

        if @button == :top
          RemoveButton(Id(:rp_top))
        elsif @button == :center
          RemoveButton(Id(:rp_center))
        elsif @button == :bottom
          RemoveButton(Id(:rp_bottom))
        end
      end until @button == :close

      UI.CloseDialog

      nil
    end

    # Return a text summary of existing buttons.
    #
    def Summary
      summary = (UI.WidgetExists(Id(:top)) ?
        "Top button exists" :
        "No top button") +
        (UI.WidgetExists(Id(:center)) ?
          "\nCenter button exists" :
          "\nNo center button") +
        (UI.WidgetExists(Id(:bottom)) ?
          "\nBottom button exists" :
          "\nNo bottom button")

      summary
    end

    # Remove button with given id and update summary.
    #
    def RemoveButton(id)
      id = deep_copy(id)
      UI.ReplaceWidget(id, Empty())
      UI.ChangeWidget(Id(:summary), :Value, Summary())
      UI.RecalcLayout

      nil
    end
  end
end

Yast::WidgetExistsClient.new.main
