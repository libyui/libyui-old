# encoding: utf-8

module Yast
  class ShortcutCheckPostponedClient < Client
    def main
      Yast.import "UI"
      # Demo for postponed shortcut checking

      @b_kilroy = PushButton(Opt(:hstretch), "&Kilroy")
      @b_was = PushButton(Opt(:hstretch), "&was")
      @b_here = PushButton(Opt(:hstretch), "&here")


      UI.OpenDialog(
        VBox(
          VSpacing(0.3),
          HBox(
            HSpacing(),
            Label("Click on one of the upper buttons\n" + "to exchange them."),
            HSpacing()
          ),
          VSpacing(0.3),
          ReplacePoint(Id(1), @b_kilroy),
          ReplacePoint(Id(2), @b_was),
          ReplacePoint(Id(3), @b_here),
          VSpacing(Opt(:vstretch)),
          Right(PushButton(Id(:cancel), "&Quit"))
        )
      )


      @button = nil

      Builtins.y2milestone("Initial state: 'Kilroy was here'")
      @button = UI.UserInput
      return if @button == :cancel

      UI.PostponeShortcutCheck # "&Kilroy" "&was" "&here"
      UI.ReplaceWidget(Id(1), @b_was) # "&was" "&was" "&here"
      UI.ReplaceWidget(Id(2), @b_kilroy) # "&was" "&Kilroy" "&here"
      UI.CheckShortcuts

      Builtins.y2milestone("After change: 'was Kilroy here'")
      @button = UI.UserInput
      return if @button == :cancel

      UI.PostponeShortcutCheck # "&was" "&Kilroy" "&here"
      UI.ReplaceWidget(Id(1), @b_here) # "&here" "&Kilroy" "&here"
      UI.ReplaceWidget(Id(2), @b_was) # "&here" "&was" "&here"
      UI.ReplaceWidget(Id(3), @b_kilroy) # "&here" "&was" "&Kilroy"
      UI.CheckShortcuts

      Builtins.y2milestone("After change: 'here was Kilroy'")
      @button = UI.UserInput
      return if @button == :cancel

      UI.PostponeShortcutCheck # "&here" "&was" "&Kilroy"
      UI.ReplaceWidget(Id(1), @b_kilroy) # "&Kilroy" "&was" "&Kilroy"
      UI.ReplaceWidget(Id(3), @b_here) # "&Kilroy" "&was" "&here"
      # Omitting UI::CheckShortcuts();

      Builtins.y2milestone("After change: back to 'Kilroy was here'")
      Builtins.y2milestone(
        "Expect complaint about missing UI::CheckShortcuts()"
      )
      @button = UI.UserInput
      return if @button == :cancel


      Builtins.y2milestone("Expect shortcut conflict: '&was' vs. '&was'")
      UI.ReplaceWidget(Id(1), @b_was) # "&was" "&was" "&here"
      UI.ReplaceWidget(Id(2), @b_kilroy) # "&was" "&Kilroy" "&here"
      Builtins.y2milestone("Expect complaint about excess UI::CheckShortcuts()")
      UI.CheckShortcuts

      Builtins.y2milestone("After change: 'was Kilroy here'")
      @button = UI.UserInput
      return if @button == :cancel


      UI.CloseDialog

      nil
    end
  end
end

Yast::ShortcutCheckPostponedClient.new.main
