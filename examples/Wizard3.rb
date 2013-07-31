# encoding: utf-8

# Advanced example of using the Wizard widget.
#
# Note: YCP applications are discouraged from using the Wizard widget directly.
# Use the Wizard module instead.
module Yast
  class Wizard3Client < Client
    def main
      Yast.import "UI"

      @base_installation_steps = [
        { "id" => "language", "label" => "Language" },
        { "id" => "proposal", "label" => "Installation Settings" },
        { "id" => "do_resize", "label" => "Perform Installation" },
        { "id" => "prepdisk", "label" => "Perform Installation" },
        { "id" => "kickoff", "label" => "Perform Installation" },
        { "id" => "rpmcopy", "label" => "Perform Installation" },
        { "id" => "finish", "label" => "Perform Installation" }
      ]

      @config_steps = [
        { "id" => "root", "label" => "Root Password" },
        { "id" => "proposal_net", "label" => "Network" },
        { "id" => "ask_net_test", "label" => "Network" },
        { "id" => "do_net_test", "label" => "Network" },
        { "id" => "you", "label" => "Online Update" },
        { "id" => "auth", "label" => "Users" },
        { "id" => "user", "label" => "Users" },
        { "id" => "suseconfig", "label" => "Clean Up" },
        { "id" => "release_notes", "label" => "Release Notes" },
        { "id" => "proposal_hw", "label" => "Device Configuration" }
      ]

      @steps = Builtins.flatten([@base_installation_steps, @config_steps])

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
      UI.WizardCommand(
        term(:ShowReleaseNotesButton, "&Release Notes", "release_notes")
      )

      UI.WizardCommand(term(:AddStepHeading, "Base Installation"))

      Builtins.foreach(@base_installation_steps) do |step|
        UI.WizardCommand(
          term(
            :AddStep,
            Ops.get_string(step, "label", ""),
            Ops.get_string(step, "id", "")
          )
        )
      end


      UI.WizardCommand(term(:AddStepHeading, "Configuration"))

      Builtins.foreach(@config_steps) do |step|
        UI.WizardCommand(
          term(
            :AddStep,
            Ops.get_string(step, "label", ""),
            Ops.get_string(step, "id", "")
          )
        )
      end


      @current_step = 0
      show_step(0)

      while true
        @button = Convert.to_symbol(UI.UserInput)

        break if @button == :abort || @button == :cancel

        if @button == :next || @button == :back
          if @button == :next &&
              Ops.less_than(Ops.add(@current_step, 1), Builtins.size(@steps))
            @current_step = Ops.add(@current_step, 1)

            if Ops.greater_than(@current_step, 2)
              UI.WizardCommand(term(:HideReleaseNotesButton))
            end
          end

          if @button == :back && Ops.greater_than(@current_step, 0)
            @current_step = Ops.subtract(@current_step, 1)
          end

          show_step(@current_step)
        end
      end

      UI.CloseDialog

      nil
    end

    def show_step(no)
      current_id = Ops.get_string(Ops.get(@steps, no), "id", "")

      UI.ReplaceWidget(
        Id(:contents),
        VBox(
          Heading("Current workflow step:"),
          VSpacing(0.5),
          Label(current_id)
        )
      )

      UI.WizardCommand(term(:SetCurrentStep, current_id))

      nil
    end
  end
end

Yast::Wizard3Client.new.main
