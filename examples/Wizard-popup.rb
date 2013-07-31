# encoding: utf-8

# Example of using the Wizard widget.
#
# Note: YCP applications are discouraged from using the Wizard widget directly.
# Use the Wizard module instead.
module Yast
  class WizardPopupClient < Client
    def main
      Yast.import "UI"
      if !UI.HasSpecialWidget(:Wizard)
        Builtins.y2error(
          "This works only with UIs that provide the wizard widget!"
        )
        return
      end

      @help_text = "<p>This is a help text.</p>" +
        "<p>It should be helpful.</p>" +
        "<p>If it isn't helpful, it should rather not be called a <i>help text</i>.</p>"

      UI.OpenDialog(
        Opt(:defaultsize),
        Wizard(
          Opt(:stepsEnabled),
          :back,
          "&Back",
          :abort,
          "Ab&ort",
          :next,
          "&Next"
        )
      )

      # UI::DumpWidgetTree();

      UI.WizardCommand(
        term(
          :SetDialogIcon,
          "/usr/share/YaST2/theme/current/icons/22x22/apps/YaST.png"
        )
      )
      UI.WizardCommand(term(:SetDialogHeading, "Wizard with Pop-up Wizard"))
      UI.WizardCommand(term(:SetHelpText, @help_text))

      UI.WizardCommand(term(:AddStepHeading, "Steps"))
      UI.WizardCommand(term(:AddStep, "Step 1", "step1"))
      UI.WizardCommand(term(:AddStep, "Step 2", "step2"))
      UI.WizardCommand(term(:AddStep, "Step 3", "step3"))

      UI.WizardCommand(term(:UpdateSteps))

      UI.ReplaceWidget(
        :contents,
        VBox(
          PushButton(Id(:doit1), "&Do Something"),
          PushButton(Id(:doit2), "Do &More"),
          PushButton(Id(:popup), "Popup &Wizard")
        )
      )

      UI.WizardCommand(term(:SetCurrentStep, "step1"))


      while true
        @event = UI.WaitForEvent

        Builtins.y2milestone("Got event: %1", @event)

        @widget = Ops.get_symbol(@event, "ID", :nil)

        break if @widget == :abort

        if @widget == :popup
          UI.OpenDialog(
            Opt(:defaultsize),
            Wizard(:bla, "", :popdown, "&Cancel", :accept, "&Acce&pt")
          )

          UI.ReplaceWidget(
            :contents,
            VBox(
              Id(:popupWizard),
              Heading("Popup Wizard"),
              VSpacing(2),
              PushButton(Id(:doit11), "&Do Something"),
              PushButton(Id(:doit12), "Do &More")
            )
          )
        end

        UI.CloseDialog if @widget == :popdown && UI.WidgetExists(:popupWizard)
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::WizardPopupClient.new.main
