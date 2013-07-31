# encoding: utf-8

module Yast
  class DownloadProgress2Client < Client
    def main
      Yast.import "UI"
      # Check for availability of special widgets required for this example

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


      # Initialize some (pretty random) demo values

      @logfile_name = "/suse/sh/.y2log"
      # string  logfile_name = "/var/log/y2log";
      @logfile_expected_size = 20 * 1024


      # Build a dialog with a download progress bar
      # and a row of some buttons below.
      #
      # The weights for will make the buttons the same size - except the last
      # one, "Close", which is deliberately set apart from the others with some
      # empty space.

      UI.OpenDialog(
        VBox(
          DownloadProgress(
            Id(:progress),
            "YaST2 log file",
            @logfile_name,
            @logfile_expected_size
          ),
          HBox(
            HWeight(1, PushButton(Id(:y2log), "YaST2 Log &File")),
            HWeight(1, PushButton(Id(:null), "&No File")),
            HWeight(1, PushButton(Id(:blurb), Opt(:default), "&Log Something")),
            HSpacing(Opt(:hstretch), 2),
            PushButton(Id(:close), "&Close")
          )
        )
      )

      # Input loop

      @button = nil
      begin
        @button = UI.UserInput

        if @button == :y2log
          # Watch the Y2 log file

          UI.ChangeWidget(Id(:progress), :Filename, @logfile_name)
          UI.ChangeWidget(Id(:progress), :ExpectedSize, @logfile_expected_size)
        elsif @button == :null
          # Watch no file - i.e. reset the progress bar

          UI.ChangeWidget(Id(:progress), :Filename, "")
          UI.ChangeWidget(Id(:progress), :ExpectedSize, 1024)
        elsif @button == :blurb
          # log something so the log file grows

          @i = 0

          while Ops.less_than(@i, 100)
            Builtins.y2milestone("Blurb - %1", @i)
            @i = Ops.add(@i, 1)
          end
        end
      end while @button != :close


      # Clean up

      UI.CloseDialog

      nil
    end
  end
end

Yast::DownloadProgress2Client.new.main
