# encoding: utf-8

# Simple tree example
module Yast
  class Tree4Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Tree(
            Id(:dest_dir),
            Opt(:notify),
            "Select destination directory:",
            [
              Item(
                Id("root"),
                "/",
                true,
                [
                  Item(
                    Id("etc"),
                    "etc",
                    [Item("opt"), Item("SuSEconfig"), Item("X11")]
                  ),
                  Item(
                    "usr",
                    false,
                    [
                      "bin",
                      "lib",
                      Item("share", ["man", "info", "emacs"]),
                      Item(Id("usr_local"), "local"),
                      Item("X11R6", ["bin", "lib", "share", "man", "etc"])
                    ]
                  ),
                  Item(Id("opt"), "opt", true, ["kde", "netscape", "Office51"]),
                  Item("home"),
                  "work",
                  Item(Id(:other), "<other>")
                ]
              )
            ]
          ),
          HBox(
            PushButton(Id(:sel_opt), Opt(:hstretch), "/&opt"),
            PushButton(Id(:sel_usr), Opt(:hstretch), "/&usr"),
            PushButton(Id(:sel_usr_local), Opt(:hstretch), "/usr/&local")
          ),
          PushButton(Id(:ok), Opt(:default), "&OK")
        )
      )

      @id = nil
      begin
        @id = UI.UserInput

        if @id == :sel_usr
          UI.ChangeWidget(:dest_dir, :CurrentItem, "usr")
        elsif @id == :sel_usr_local
          UI.ChangeWidget(:dest_dir, :CurrentItem, "usr_local")
        elsif @id == :sel_opt
          UI.ChangeWidget(:dest_dir, :CurrentItem, "opt")
        end
      end until @id == :ok

      # Get the input from the tree.
      #
      # Notice: The return value of UI::UserInput() does NOT return this value!
      # Rather, it returns the ID of the widget (normally the PushButton)
      # that caused UI::UserInput() to return.
      @dest_dir = Convert.to_string(UI.QueryWidget(:dest_dir, :CurrentItem))
      Builtins.y2milestone("Selected: %1", @dest_dir)


      @dest_dir = "" if @dest_dir == nil

      # Close the dialog.
      # Remember to read values from the dialog's widgets BEFORE closing it!
      UI.CloseDialog


      # Pop up a new dialog to echo the selection.
      UI.OpenDialog(
        VBox(
          Label(Ops.add("Selected destination directory: ", @dest_dir)),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput

      UI.CloseDialog

      nil
    end
  end
end

Yast::Tree4Client.new.main
