# encoding: utf-8

# Demo for common ButtonBox-based dialogs
module Yast
  class ButtonBox2Client < Client
    def main
      Yast.import "UI"




      @buttons = VBox(
        PushButton(Id(:okCancel), Opt(:hstretch), "&OK / Cancel"),
        PushButton(Id(:yesNo), Opt(:hstretch), "&Yes / No"),
        PushButton(Id(:continueCancel), Opt(:hstretch), "C&ontinue / Cancel"),
        PushButton(Id(:okApply), Opt(:hstretch), "OK / &Apply / Cancel"),
        PushButton(
          Id(:okEtcHelp),
          Opt(:hstretch),
          "OK / Apply / Cancel / &Help"
        ),
        PushButton(
          Id(:okCustom),
          Opt(:hstretch),
          "OK / Apply / Cancel / C&ustom / Help"
        ),
        PushButton(Id(:okOnly), Opt(:hstretch), "O&K"),
        PushButton(Id(:okRetry), Opt(:hstretch), "OK / Re&try (Error!)"),
        PushButton(Id(:retryCancel), Opt(:hstretch), "&Retry / Cancel")
      )

      UI.OpenDialog(
        MarginBox(
          1.0,
          0.5,
          VBox(
            Heading("ButtonBox Examples"),
            VSpacing(0.5),
            HVCenter(HSquash(@buttons)),
            VSpacing(0.5),
            Right(PushButton(Id(:close), "&Close"))
          )
        )
      )

      @button = nil
      begin
        @button = Convert.to_symbol(UI.UserInput)

        okCancelDialog if @button == :okCancel
        yesNoDialog if @button == :yesNo
        continueCancelDialog if @button == :continueCancel
        okApplyCancelDialog if @button == :okApply
        okApplyCancelHelpDialog if @button == :okEtcHelp
        okApplyCancelCustomHelpDialog if @button == :okCustom
        okDialog if @button == :okOnly
        okRetryDialog if @button == :okRetry
        retryCancelDialog if @button == :retryCancel
      end until @button == :close || @button == :cancel # [Close] button # WM_CLOSE

      UI.CloseDialog

      nil
    end

    def showDialog(buttonBox)
      buttonBox = deep_copy(buttonBox)
      success = UI.OpenDialog(VBox(HVCenter(Label("Hello, World!")), buttonBox))

      # Most YCP developers never use the return value of UI::OpenDialog().
      # Many of them probably don't even know that it has a return value.
      #
      # Used properly, that return value can be used to recover from error
      # situations that would otherwise abort the program - like in this case.

      if success
        UI.UserInput
        UI.CloseDialog
      end

      nil
    end


    def okCancelDialog
      showDialog(
        ButtonBox(
          PushButton(Id(:ok), "&OK"),
          PushButton(Id(:cancel), "&Cancel")
        )
      )

      nil
    end


    def yesNoDialog
      showDialog(
        ButtonBox(PushButton(Id(:yes), "&Yes"), PushButton(Id(:no), "&No"))
      )

      nil
    end


    def continueCancelDialog
      showDialog(
        ButtonBox(
          PushButton(Id(:continue), "C&ontinue"),
          PushButton(Id(:cancel), "&Cancel")
        )
      )

      nil
    end


    def okApplyCancelDialog
      showDialog(
        ButtonBox(
          PushButton(Id(:ok), "&OK"),
          PushButton(Id(:apply), "&Apply"),
          PushButton(Id(:cancel), "&Cancel")
        )
      )

      nil
    end


    def okApplyCancelHelpDialog
      showDialog(
        ButtonBox(
          PushButton(Id(:ok), "&OK"),
          PushButton(Id(:apply), "&Apply"),
          PushButton(Id(:cancel), "&Cancel"),
          PushButton(Id(:help), "&Help")
        )
      )

      nil
    end


    def okApplyCancelCustomHelpDialog
      showDialog(
        ButtonBox(
          PushButton(Id(:ok), "&OK"),
          PushButton(Id(:apply), "&Apply"),
          PushButton(Id(:cancel), "&Cancel"),
          PushButton(Id(:custom1), "&Do Something"),
          PushButton(Id(:custom2), "Do &More"),
          PushButton(Id(:help), "&Help")
        )
      )

      nil
    end


    def okDialog
      showDialog(ButtonBox(PushButton(Id(:ok), "&OK")))

      nil
    end


    def okRetryDialog
      # This will throw an error:
      # If there is more than one button in a button box, one of them must
      # have the [OK] role and one must have the [Cancel] role.

      showDialog(
        ButtonBox(PushButton(Id(:ok), "&OK"), PushButton(Id(:retry), "&Retry"))
      )

      nil
    end


    def retryCancelDialog
      # Explicitly assigning the [OK] role to the [Retry] button

      showDialog(
        ButtonBox(
          PushButton(Id(:retry), Opt(:okButton), "&Retry"),
          PushButton(Id(:cancel), "&Cancel")
        )
      )

      nil
    end
  end
end

Yast::ButtonBox2Client.new.main
