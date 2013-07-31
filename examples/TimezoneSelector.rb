# encoding: utf-8

module Yast
  class TimezoneSelectorClient < Client
    def main
      Yast.import "UI"
      Yast.import "Directory"

      # Build a dialog with a "special" widget - one that may not be supported
      # by all UIs.

      @examples = {
        "Europe/Amsterdam"    => "Netherlands",
        "Europe/Athens"       => "Greece",
        "Europe/Berlin"       => "Germany",
        "Europe/Bratislava"   => "Slovakia",
        "Europe/Brussels"     => "Belgium",
        "Europe/Helsinki"     => "Finland",
        "Europe/Istanbul"     => "Turkey",
        # time zone
        "Europe/Kaliningrad"  => "Russia (Kaliningrad)",
        "Europe/Kiev"         => "Ukraine",
        "Europe/Malta"        => "Malta",
        # time zone
        "Europe/Minsk"        => "Belarus",
        "Europe/Monaco"       => "Monaco",
        # time zone
        "Europe/Moscow"       => "Russia (Moscow)",
        "Europe/Oslo"         => "Norway",
        "Europe/Paris"        => "France",
        "Europe/Prague"       => "Czech Republic",
        "Europe/Riga"         => "Latvia",
        "Europe/Rome"         => "Italy",
        # time zone
        "Europe/San_Marino"   => "San Marino",
        # time zone
        "Europe/Samara"       => "Russia (Samara)",
        "Europe/Sarajevo"     => "Bosnia & Herzegovina",
        "US/Aleutian"         => "Aleutian",
        "US/Arizona"          => "Arizona",
        "US/Central"          => "Central",
        "US/East-Indiana"     => "East Indiana",
        "US/Hawaii"           => "Hawaii",
        "US/Indiana-Starke"   => "Indiana Starke",
        "US/Michigan"         => "Michigan",
        "US/Mountain"         => "Mountain",
        "US/Pacific"          => "Pacific",
        "US/Samoa"            => "Samoa",
        "US/Eastern"          => "Eastern",
        "Europe/London"       => "United Kingdom",
        "America/New_York"    => "Eastern",
        "Australia/Darwin"    => "Northern Territory--Darwin",
        "Australia/Brisbane"  => "Queensland--Brisbane",
        "Australia/South"     => "South Australia--Adelaide",
        "Australia/Sydney"    => "New South Wales--Sydney",
        "Australia/ACT"       => "Australian Capital Territory--Canberra",
        "Australia/Hobart"    => "Tasmania--Hobart",
        "Australia/Melbourne" => "Victoria--Melbourne",
        "Australia/Perth"     => "Western Australia--Perth"
      }


      # Ask the UI whether or not it supports this widget.

      if UI.HasSpecialWidget(:TimezoneSelector)
        # Only create a dialog with this kind of widget if it is supported
        UI.OpenDialog(
          VBox(
            TimezoneSelector(
              Id(:timezone),
              Opt(:notify),
              Ops.add(Directory.themedir, "/current/worldmap/worldmap.jpg"),
              @examples
            ),
            PushButton(Id(:ok), Opt(:default), "&Close"),
            ComboBox(
              Id(:combozone),
              Opt(:notify),
              "",
              [
                Item(Id(:london), "Europe/London"),
                Item(Id(:prag), "Europe/Prague"),
                Item(Id(:ny), "America/New_York")
              ]
            )
          )
        )
        # preselect something
        UI.ChangeWidget(Id(:timezone), :CurrentItem, "Europe/London")
      else
        # Always provide a fallback: Either try to create a simpler dialog
        # without the special widget, or terminate with an error message.
        UI.OpenDialog(
          VBox(
            Label("Timezone Selector not supported!"),
            PushButton(Opt(:default), "&Oops!")
          )
        )
      end

      while true
        @button = Convert.to_symbol(UI.UserInput)
        break if @button == :ok || @button == :cancel

        if @button == :combozone
          @zone = UI.QueryWidget(Id(:combozone), :Value)
          if @zone == :london
            UI.ChangeWidget(Id(:timezone), :Value, "Europe/London")
          end
          if @zone == :prag
            UI.ChangeWidget(Id(:timezone), :Value, "Europe/Prague")
          end
          if @zone == :ny
            UI.ChangeWidget(Id(:timezone), :Value, "America/New_York")
          end
        end

        if @button == :timezone
          @zone = Convert.to_string(UI.QueryWidget(Id(:timezone), :Value))
          UI.ChangeWidget(Id(:combozone), :Value, @zone)
        end
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::TimezoneSelectorClient.new.main
