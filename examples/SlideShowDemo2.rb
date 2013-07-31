# encoding: utf-8

# SlideShow demo: Using Wizard and MultiProgressMeter
#
# Note: YCP applications are discouraged from using the Wizard widget directly.
# Use the Wizard module instead.
module Yast
  class SlideShowDemo2Client < Client
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

      @initialRpms = [600, 150, 30, 100]
      @rpms = deep_copy(@initialRpms)
      @useTimeout = false
      @currentCd = 1
      @megaBytesPerSecond = 2



      #
      # ----------------------------- main() ----------------------------------
      #


      if !UI.HasSpecialWidget(:Wizard) || !UI.HasSpecialWidget(:DumbTab)
        Builtins.y2error(
          "This works only with UIs that provide the Wizard and the DumbTab widget!"
        )
        return
      end

      @help_text = "<p>Please wait while packages are being installed.</p>"
      @slide_text = "\t\t\t\t\t\t\t\t<table cellspacing=\"10\" cellpadding=\"5\">\t\t\t\t\t\t<tr>\t\t\t\t\t\t\t\t\t\t\t<td width =\"*\">\t\t\t\t\t\t\t\t\t<img src=\"/usr/share/YaST2/theme/current/icons/48x48/apps/yast-sound.png\"\t\talign=\"left\">\t\t\t\t\t\t\t\t\t\t</td>\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t<td width=\"*\">\t\t\t\t\t\t\t\t\t<p><font color=\"#00007f\"><b>XMMS and JuK - Powerful Jukeboxes</b></font></p>\t\t<p>XMMS is an excellent sound player for Linux. It is easy to use and supports\t\tvarious formats, including audio CDs. Test the many visualization plug-ins or\t\tdownload your favorite XMMS skins from the web.</p>\t\t\t\t\t<p>New in KDE: JuK, which classifies your MP3s and organizes your music\t\t\tcollection.</p>\t\t\t\t\t\t\t\t\t\t<p><b>Want More?</b></p>\t\t\t\t\t\t\t\t<p>The SUSE distribution features a wide range of applications for playing\t\tyour CDs and MP3 songs. For example, KsCD is a user-friendly CD player. The\t\ttrack information for most CDs is available on the Internet. Simply activate\t\tthe respective function to display the list.</p>\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t</td>\t\t\t\t\t\t\t\t\t\t\t</tr>\t\t\t\t\t\t\t\t\t\t\t</table>"


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
      UI.WizardCommand(term(:SetDialogHeading, "Package Installation"))
      UI.WizardCommand(term(:SetHelpText, @help_text))

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

      UI.WizardCommand(term(:SetCurrentStep, "rpmcopy"))
      UI.WizardCommand(
        term(:SetNextButtonLabel, @useTimeout ? "&Stop" : "&Start")
      )
      UI.WizardCommand(term(:SetBackButtonLabel, "S&tep"))

      @slideShowPage = RichText(Id(:slideText), @slide_text)


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

      @detailsPage = VBox(
        Id(:detailsPage),
        VBox(
          VWeight(1, @cdTable),
          VWeight(1, LogView(Id(:pkgLog), "", 6, 100)),
          ProgressBar(
            Id(:progressCurrentPackage),
            "hypercool-1.3.7 (143.3 MB)",
            100,
            42
          )
        )
      )


      UI.ReplaceWidget(
        Id(:contents),
        VBox(
          VWeight(
            1, # lower layout priority
            HBox(
              DumbTab(
                [
                  Item(Id(:showSlideShow), "Slide Sho&w"),
                  Item(Id(:showDetails), "&Details")
                ],
                VBox(
                  VSpacing(0.4),
                  VWeight(
                    1, # lower layout priority
                    HBox(
                      HSpacing(1),
                      ReplacePoint(Id(:tabContents), @slideShowPage),
                      HSpacing(0.5)
                    )
                  ),
                  VSpacing(0.4)
                )
              ),
              HSpacing(0.5),
              VBox(
                Label(Id(:totalRemaining), totalRemainingTime),
                VWeight(1, VMultiProgressMeter(Id(:progress), @rpms))
              )
            )
          ),
          VSpacing(0.3),
          Label(Id(:nextMedia), Opt(:hstretch), "")
        )
      )

      while true
        @button = @useTimeout ?
          Convert.to_symbol(UI.TimeoutUserInput(100)) :
          Convert.to_symbol(UI.UserInput)

        break if @button == :abort

        @delta = 0
        if @button == :next
          @useTimeout = !@useTimeout
          UI.WizardCommand(
            term(:SetNextButtonLabel, @useTimeout ? "&Stop" : "&Start")
          )

          @currentCd = 1 if Ops.less_than(@currentCd, 0)
        end
        if @button == :back
          @delta = 10
        elsif @button == :timeout
          @delta = 100
        elsif @button == :showSlideShow
          UI.ReplaceWidget(:tabContents, @slideShowPage)
        elsif @button == :showDetails
          Builtins.y2milestone("detailsPage:\n%1", @detailsPage)
          UI.ReplaceWidget(:tabContents, @detailsPage)


          UI.ChangeWidget(
            :pkgLog,
            :LastLine,
            Ops.add(
              Ops.add(
                Ops.add(
                  Ops.add(
                    pkgInfo(
                      "ClosedOffice",
                      "0.8.3",
                      830.7,
                      "The infamouse office suite"
                    ),
                    "\n"
                  ),
                  pkgInfo("cool-macs", "27.9.1", 250.3, "An editor-like OS")
                ),
                "\n"
              ),
              pkgInfo("hypercool", "1.3.7", 147.3, "A hyper cool gizmofier")
            )
          )
          UI.RecalcLayout
        end

        if Ops.greater_than(@delta, 0)
          @rpms = subtract(@rpms, 10)
          UI.ChangeWidget(:totalRemaining, :Value, totalRemainingTime)
          UI.ChangeWidget(:nextMedia, :Value, nextMedia)
          UI.RecalcLayout
        end
      end

      UI.CloseDialog

      nil
    end

    def mediaName(mediaNo)
      Builtins.sformat("SuSE Linux Professional CD %1", mediaNo)
    end


    def mediaChange(cdNo)
      UI.ChangeWidget(:nextMedia, :Value, "")
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


    def subtract(valueList, delta)
      valueList = deep_copy(valueList)
      subtracted = false
      newList = []
      cdNo = 1
      newCd = -1
      total = 0

      Builtins.foreach(valueList) do |val|
        cdNo = Ops.add(cdNo, 1)
        total = Ops.add(total, val)
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

      UI.ChangeWidget(:progress, :Values, newList)

      if Ops.greater_than(newCd, 0) &&
          Ops.less_or_equal(newCd, Builtins.size(valueList))
        mediaChange(newCd)
      end

      if Ops.less_or_equal(total, 0)
        @useTimeout = false
        UI.WizardCommand(term(:SetNextButtonLabel, "&Start"))
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


    def totalRemainingTime
      left = 0

      Builtins.foreach(@rpms) { |val| left = Ops.add(left, val) }

      totalSeconds = Ops.multiply(left, @megaBytesPerSecond)

      Builtins.sformat("Remaining:\n%1", formatTime(totalSeconds))
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

Yast::SlideShowDemo2Client.new.main
