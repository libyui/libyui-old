# encoding: utf-8

module Yast
  class Tree3Client < Client
    def main
      Yast.import "UI"
      # Build a dialog with a tree for directory selection, three
      # buttons with common values and a label that directly echoes any
      # selected directory.
      #
      # The tree in this example uses the `notify option that makes
      # UI::UserInput() return immediately as soon as the user selects a
      # tree item rather than the default behaviour which waits for the
      # user to activate a button.

      UI.OpenDialog(
        MinWidth(
          50,
          VBox(
            Tree(
              Id(:dest_dir),
              Opt(:notify),
              "Select destination directory:",
              [
                Item(
                  "/",
                  true,
                  [
                    Item("etc", [Item("opt"), Item("SuSEconfig"), Item("X11")]),
                    Item(
                      "usr",
                      false,
                      [
                        "bin",
                        "lib",
                        Item("share", ["man", "info", "emacs"]),
                        Item("local"),
                        Item("X11R6", ["bin", "lib", "share", "man", "etc"])
                      ]
                    ),
                    Item("opt", true, ["kde", "netscape", "Office51"]),
                    Item("home"),
                    "work",
                    Item("<other>")
                  ]
                )
              ]
            ),
            HBox(
              PushButton(Id(:sel_opt), Opt(:hstretch), "/&opt"),
              PushButton(Id(:sel_usr), Opt(:hstretch), "/&usr"),
              PushButton(Id(:sel_usr_local), Opt(:hstretch), "/usr/&local")
            ),
            HBox(
              HWeight(2, Label("Current Item:")),
              HWeight(5, Label(Id(:echoItem), Opt(:outputField, :hstretch), ""))
            ),
            HBox(
              HWeight(2, Label("Current Branch:")),
              HWeight(
                5,
                Label(Id(:echoBranch), Opt(:outputField, :hstretch), "")
              )
            ),
            HBox(
              HWeight(2, Label("Current Path:")),
              HWeight(5, Label(Id(:echoPath), Opt(:outputField, :hstretch), ""))
            ),
            PushButton(Id(:ok), Opt(:default), "&OK")
          )
        )
      )

      @id = nil
      begin
        @id = UI.UserInput

        if @id == :sel_usr
          UI.ChangeWidget(Id(:dest_dir), :CurrentItem, "usr")
        elsif @id == :sel_usr_local
          UI.ChangeWidget(Id(:dest_dir), :CurrentItem, "local")
        elsif @id == :sel_opt
          UI.ChangeWidget(Id(:dest_dir), :CurrentItem, "opt")
        elsif @id == :dest_dir
          @current_dir = UI.QueryWidget(:dest_dir, :CurrentItem)

          if @current_dir != nil
            UI.ChangeWidget(
              Id(:echoItem),
              :Value,
              Builtins.sformat("%1", @current_dir)
            )
          end

          @current_branch = Convert.convert(
            UI.QueryWidget(:dest_dir, :CurrentBranch),
            :from => "any",
            :to   => "list <string>"
          )

          if @current_branch != nil
            UI.ChangeWidget(
              :echoBranch,
              :Value,
              Builtins.sformat("%1", @current_branch)
            )

            @current_path = Builtins.mergestring(@current_branch, "/")
            if Ops.greater_than(Builtins.size(@current_path), 2)
              # Remove duplicate "/" at start
              @current_path = Builtins.substring(
                @current_path,
                1,
                Ops.subtract(Builtins.size(@current_path), 1)
              )
            end

            UI.ChangeWidget(
              :echoPath,
              :Value,
              Builtins.sformat("%1", @current_path)
            )
          end
        end
      end until @id == :ok


      # Close the dialog.
      # Remember to read values from the dialog's widgets BEFORE closing it!
      UI.CloseDialog

      nil
    end
  end
end

Yast::Tree3Client.new.main
