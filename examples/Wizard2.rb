# encoding: utf-8

# Example of using the Wizard widget.
#
# Note: YCP applications are discouraged from using the Wizard widget directly.
# Use the Wizard module instead.
module Yast
  class Wizard2Client < Client
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
      UI.WizardCommand(
        term(:SetDialogHeading, "Welcome to the YaST2 installation")
      )
      UI.WizardCommand(term(:SetHelpText, @help_text))

      UI.WizardCommand(term(:AddStepHeading, "Base Installation"))
      UI.WizardCommand(term(:AddStep, "Language", "lang"))
      UI.WizardCommand(term(:AddStep, "Installation Settings", "proposal"))
      UI.WizardCommand(term(:AddStep, "Perform Installation", "doit"))

      UI.WizardCommand(term(:AddStepHeading, "Configuration"))
      UI.WizardCommand(term(:AddStep, "Root Password", "root_pw"))
      UI.WizardCommand(term(:AddStep, "Network", "net"))
      UI.WizardCommand(term(:AddStep, "Online Update", "you"))
      UI.WizardCommand(term(:AddStep, "Users", "auth"))
      UI.WizardCommand(term(:AddStep, "Clean Up", "suse_config"))
      UI.WizardCommand(term(:AddStep, "Release Notes", "rel_notes"))
      UI.WizardCommand(term(:AddStep, "Device Configuration", "hw_proposal"))
      UI.WizardCommand(term(:UpdateSteps))

      if false
        UI.WizardCommand(term(:SetAbortButtonLabel, "&Cancel"))
        UI.WizardCommand(term(:SetBackButtonLabel, ""))
        UI.WizardCommand(term(:SetNextButtonLabel, "&Accept"))
      end

      UI.WizardCommand(term(:SetCurrentStep, "net"))

      while true
        @event = UI.WaitForEvent

        Builtins.y2milestone("Got event: %1", @event)

        break if Ops.get(@event, "ID") == :abort

        @serial = Ops.get_integer(@event, "EventSerialNo", 0)
        @type = Ops.get_string(@event, "EventType", "")
        @id = Ops.get_symbol(@event, "ID", :nil)


        UI.ReplaceWidget(
          Id(:contents),
          VBox(
            Heading("Caught event:"),
            VSpacing(0.5),
            Label(Ops.add("Serial No: ", Builtins.tostring(@serial))),
            Label(Ops.add("Type: ", @type)),
            Label(Ops.add("ID: ", Builtins.tostring(@id)))
          )
        )
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::Wizard2Client.new.main
