# encoding: utf-8

# Example for the file selection builtins
module Yast
  class FileSelectionClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          Heading("YaST2 File Selector Demo"),
          VStretch(),
          HBox(
            HStretch(),
            HWeight(
              1,
              VBox(
                PushButton(Id(:askDir), Opt(:hstretch), "Select &Directory..."),
                PushButton(Id(:load), Opt(:hstretch), "&Load File..."),
                PushButton(Id(:saveAs), Opt(:hstretch), "Save &As...")
              )
            ),
            HSpacing(2)
          ),
          VSpacing(),
          HBox(
            Label("Selected: "),
            Label(Id(:name), Opt(:outputField, :hstretch), "<nothing selected>")
          ),
          VSpacing(),
          Right(PushButton(Id(:close), "&Close"))
        )
      )

      @button = nil
      @name = ""
      begin
        @update = false
        @button = UI.UserInput

        if @button == :askDir
          @name = UI.AskForExistingDirectory("/tmp", "Select Directory")
        elsif @button == :load
          @name = UI.AskForExistingFile(
            "/usr/X11R6/share",
            "*.jpg *.png",
            "Select an image to load"
          )
        elsif @button == :saveAs
          @name = UI.AskForSaveFileName("/tmp", "*", "Save as...")
        end

        @name = "<canceled>" if @name == nil

        if @name != "" # Label update necessary?
          UI.ChangeWidget(Id(:name), :Value, @name)
          UI.RecalcLayout
          @name = ""
        end
      end until @button == :close || @button == :cancel

      UI.CloseDialog

      nil
    end
  end
end

Yast::FileSelectionClient.new.main
