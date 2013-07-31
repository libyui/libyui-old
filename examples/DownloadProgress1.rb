# encoding: utf-8

module Yast
  class DownloadProgress1Client < Client
    def main
      Yast.import "UI"
      if !UI.HasSpecialWidget(:DownloadProgress)
        UI.OpenDialog(
          VBox(
            Label("Error: This UI doesn't support the DownloadProgress widget!"),
            PushButton(Opt(:default), "&OK")
          )
        )
        UI.UserInput
        UI.CloseDialog

        return
      end

      @filename = "/suse/sh/.y2log"
      # string  filename = "/var/log/y2log";
      @expected_size = 20 * 1024

      UI.OpenDialog(
        VBox(
          DownloadProgress("YaST2 log file", @filename, @expected_size),
          HSpacing(50), # force width
          PushButton(Opt(:default), "&Close")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::DownloadProgress1Client.new.main
