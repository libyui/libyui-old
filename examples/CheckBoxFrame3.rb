# encoding: utf-8

# Example for CheckBoxFrame without auto enable:
# The application has to handle the check box
module Yast
  class CheckBoxFrame3Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          MarginBox(
            1,
            0.5,
            CheckBoxFrame(
              Id(:use_suse_server),
              Opt(:noAutoEnable, :notify),
              "&SuSE Server",
              false,
              VBox(
                HBox(
                  InputField(Id(:server), "&Server"),
                  ComboBox(Id(:mode), "&Mode", ["Automatic", "Manual", "Debug"])
                ),
                Left(Id(:logging), CheckBox("&Logging")),
                InputField(Id(:connections), "&Connections")
              )
            )
          ),
          PushButton(Id(:ok), "&OK")
        )
      )

      @widget = nil
      @old_server = ""
      UI.FakeUserInput(:use_suse_server)
      begin
        @widget = Convert.to_symbol(UI.UserInput)

        if @widget == :use_suse_server
          Builtins.y2debug("Changing enabled states")
          @use_suse_server = Convert.to_boolean(
            UI.QueryWidget(:use_suse_server, :Value)
          )
          UI.ChangeWidget(:server, :Enabled, !@use_suse_server)
          UI.ChangeWidget(:mode, :Enabled, !@use_suse_server)

          if @use_suse_server
            @old_server = Convert.to_string(UI.QueryWidget(:server, :Value))
            UI.ChangeWidget(:server, :Value, "ftp://ftp.opensuse.org")
          else
            UI.ChangeWidget(:server, :Value, @old_server)
          end
        end
      end until @widget == :ok || @widget == :cancel

      UI.CloseDialog

      nil
    end
  end
end

Yast::CheckBoxFrame3Client.new.main
