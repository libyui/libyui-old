# encoding: utf-8

# NCurses SlideShow demo: No Wizard or MultiProgressMeter widget available,
# thus using simpler layout
module Yast
  class NCursesSlideShowDemoClient < Client
    def main
      Yast.import "UI"
      Yast.import "Wizard"

      @initialRpms = [600, 150, 30, 100]
      @rpms = deep_copy(@initialRpms)
      @totalToInstall = 0
      @useTimeout = false
      @currentCd = 1
      @megaBytesPerSecond = 2



      #
      # ----------------------------- main() ----------------------------------
      #



      @help_text = "<p>Please wait while packages are being installed.</p>"
      @slide_text = "\t\t\t\t\t\t\t\t<table cellspacing=\"10\" cellpadding=\"5\">\t\t\t\t\t\t<tr>\t\t\t\t\t\t\t\t\t\t\t<td width =\"*\">\t\t\t\t\t\t\t\t\t<img src=\"/opt/kde3/share/icons/crystalsvg/128x128/apps/kscd.png\"  width=\"100\"\talign=\"left\">\t\t\t\t\t\t\t\t\t\t</td>\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t<td width=\"*\">\t\t\t\t\t\t\t\t\t<p><font color=\"#00007f\"><b>XMMS and JuK - Powerful Jukeboxes</b></font></p>\t\t<p>XMMS is an excellent sound player for Linux. It is easy to use and supports\t\tvarious formats, including audio CDs. Test the many visualization plug-ins or\t\tdownload your favorite XMMS skins from the web.</p>\t\t\t\t\t<p>New in KDE: JuK, which classifies your MP3s and organizes your music\t\t\tcollection.</p>\t\t\t\t\t\t\t\t\t\t<p><b>Want More?</b></p>\t\t\t\t\t\t\t\t<p>The SUSE distribution features a wide range of applications for playing\t\tyour CDs and MP3 songs. For example, KsCD is a user-friendly CD player. The\t\ttrack information for most CDs is available on the Internet. Simply activate\t\tthe respective function to display the list.</p>\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t</td>\t\t\t\t\t\t\t\t\t\t\t</tr>\t\t\t\t\t\t\t\t\t\t\t</table>"


      @cdTable = Table(
        Id(:cdTable),
        Opt(:keepSorting),
        Header("Media", "Size", "Packages", "Time"),
        [
          Item(Id("total"), "Total", "638.1 MB", "485", "23:31"),
          Item(Id("1-1"), "SLES-9 Base CD", "620.7 MB", "480", "20:23"),
          Item(Id("1-1"), "SLES-9 CD 1", "620.7 MB", "480", "20:23"),
          Item(Id("1-2"), "SLES-9 CD 2", "17.4 MB", "5", "3:08"),
          Item(Id("1-3"), "SLES-9 CD 3", "--", "--", "--:--"),
          Item(Id("1-4"), "SLES-9 CD 4", "--", "--", "--:--"),
          Item(Id("1-5"), "SLES-9 CD 5", "--", "--", "--:--"),
          Item(Id("2-1"), "SLES-9 SP 1 CD 1", "--", "--", "--:--"),
          Item(Id("2-2"), "SLES-9 SP 1 CD 2", "--", "--", "--:--"),
          Item(Id("3-1"), "SLES-9 SP 2 CD 1", "--", "--", "--:--")
        ]
      )

      @totalToInstall = listSum(@initialRpms)

      @detailsPage = VBox(
        Id(:detailsPage), #, `VSpacing( 0.3 ),
        # `Label(`id(`nextMedia ), `opt( `hstretch), "" ) // Reserve space
        VWeight(1, @cdTable),
        VWeight(1, LogView(Id(:pkgLog), "", 6, 100)),
        ProgressBar(
          Id(:progressCurrentPackage),
          "hypercool-1.3.7 (143.3 MB)",
          100,
          42
        ),
        ProgressBar(Id(:progressTotal), "Total", @totalToInstall, 0)
      )


      @contents = VBox(
        VSpacing(0.4),
        VWeight(
          1, # lower layout priority
          HBox(HSpacing(1), @detailsPage, HSpacing(0.5))
        ),
        VSpacing(0.4)
      )

      Wizard.OpenNextBackDialog
      Wizard.SetContents(
        "Package Installation",
        @contents,
        @help_text,
        true,
        true
      )
      Wizard.SetNextButton(:startStop, @useTimeout ? "&Stop" : "&Start")
      Wizard.SetBackButton(:step, "S&tep")

      #	UI::ChangeWidget(`nextMedia, `Value, nextMedia() + "      " ); // Reserve space
      #	UI::RecalcLayout();
      #	UI::ChangeWidget(`nextMedia, `Value, "" );	//
      #	// Intentionally omitting UI::RecalcLayout() so the reserved space remains


      while true
        @button = @useTimeout ?
          Convert.to_symbol(UI.TimeoutUserInput(100)) :
          Convert.to_symbol(UI.UserInput)

        break if @button == :abort

        @delta = 0
        if @button == :startStop
          @useTimeout = !@useTimeout
          Wizard.SetNextButton(:startStop, @useTimeout ? "&Stop" : "&Start")
          UI.RecalcLayout

          @currentCd = 1 if Ops.less_than(@currentCd, 0)
        end
        if @button == :step
          @delta = 10
        elsif @button == :timeout
          @delta = 100
        end

        if Ops.greater_than(@delta, 0)
          @rpms = install(@rpms, 10)

          if UI.WidgetExists(:nextMedia)
            UI.ChangeWidget(:nextMedia, :Value, nextMedia)
            UI.RecalcLayout
          end
        end
      end

      UI.CloseDialog

      nil
    end

    def mediaName(mediaNo)
      Builtins.sformat("SuSE Linux Professional CD %1", mediaNo)
    end


    def mediaChange(cdNo)
      UI.ChangeWidget(:nextMedia, :Value, "") if UI.WidgetExists(:nextMedia)

      UI.OpenDialog(
        VBox(
          Label(Builtins.sformat("Please insert \n%1", mediaName(cdNo))),
          PushButton(Opt(:default), "&OK")
        )
      )
      # UI::TimeoutUserInput( 5 * 1000 );
      UI.UserInput
      UI.CloseDialog
      @currentCd = cdNo

      nil
    end


    def listSum(valueList)
      valueList = deep_copy(valueList)
      sum = 0

      Builtins.foreach(valueList) { |val| sum = Ops.add(sum, val) }

      sum
    end


    def install(valueList, delta)
      valueList = deep_copy(valueList)
      subtracted = false
      newList = []
      cdNo = 1
      newCd = -1
      totalRemaining = 0

      Builtins.foreach(valueList) do |val|
        cdNo = Ops.add(cdNo, 1)
        totalRemaining = Ops.add(totalRemaining, val)
        if Ops.greater_than(val, 0) && !subtracted
          val = Ops.subtract(val, delta)
          subtracted = true

          if Ops.less_or_equal(val, 0)
            val = 0
            newCd = cdNo
          end
        end
        newList = Builtins.add(newList, val)
      end

      UI.ChangeWidget(
        :progressTotal,
        :Value,
        Ops.subtract(@totalToInstall, totalRemaining)
      )

      if Ops.greater_than(newCd, 0) &&
          Ops.less_or_equal(newCd, Builtins.size(valueList))
        mediaChange(newCd)
      end

      if Ops.less_or_equal(totalRemaining, 0)
        @useTimeout = false
        Wizard.SetNextButton(:startStop, "&Start")
        newList = deep_copy(@initialRpms)
        @currentCd = -1
        Builtins.y2milestone("Resetting to %1", @rpms)
      end

      deep_copy(newList)
    end


    def twoDigits(n)
      Ops.less_than(n, 10) ?
        Builtins.sformat("0%1", n) :
        Builtins.sformat("%1", n)
    end


    def formatTime(seconds)
      hours = Ops.divide(seconds, 3600)
      minutes = Ops.divide(seconds, 60)
      seconds = Ops.modulo(seconds, 60)
      Builtins.sformat(
        "%1:%2:%3",
        hours,
        twoDigits(minutes),
        twoDigits(seconds)
      )
    end


    def nextMedia
      remaining = ""
      mediaType = "CD"

      if Ops.greater_than(@currentCd, 0) &&
          Ops.greater_than(Builtins.size(@rpms), @currentCd)
        seconds = Ops.multiply(
          Ops.get(@rpms, Ops.subtract(@currentCd, 1), 0),
          @megaBytesPerSecond
        )
        remaining = Builtins.sformat(
          "Next %1: %2  --  %3",
          mediaType,
          mediaName(Ops.add(@currentCd, 1)),
          formatTime(seconds)
        )
      end

      remaining
    end


    def pkgInfo(pkgName, pkgVersion, pkgSizeMB, pkgSummary)
      pkgSizeMB = deep_copy(pkgSizeMB)
      info = Builtins.sformat("%1-%2 (%3 MB)", pkgName, pkgVersion, pkgSizeMB)

      if Ops.greater_than(Builtins.size(pkgSummary), 0)
        info = Ops.add(Ops.add(info, " - "), pkgSummary)
      end

      info
    end
  end
end

Yast::NCursesSlideShowDemoClient.new.main
