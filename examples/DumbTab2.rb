# encoding: utf-8

# Typical usage example for tab widget
module Yast
  class DumbTab2Client < Client
    def main
      Yast.import "UI"
      @address_page = VBox(
        Left(Heading("Address")),
        VSpacing(),
        HCenter(
          HSquash(
            VBox(
              HSpacing(50),
              InputField("Name"),
              InputField("E-Mail"),
              InputField("Phone"),
              VSpacing(),
              MultiLineEdit("Comments"),
              VStretch()
            )
          )
        )
      )

      @overview_page = VBox(
        Left(Heading("DumbTab Widget Overview")),
        VSpacing(),
        Label(
          "This kind of tab is pretty dumb - hence the name DumbTab.\n" +
            "You need to do most everything yourself.\n" +
            "Each tab behaves very much like a push button;\n" +
            "the YCP application is notified when the user clicks on a tab.\n" +
            "The application must take care to exchange the tab contents.\n" + "\n" +
            "Note: That means changes made in on tab are lost when switching\n" +
            "the tabs, e.g. text entered here in the address tab."
        )
      )

      @style_hints_page = VBox(
        Left(Heading("GUI Style Hints")),
        VSpacing(),
        Heading("Using tabs is usually a result of poor dialog design."),
        VSpacing(),
        Left(
          Label(
            "Tabs hide complexity, they do not resolve it.\n" +
              "The problem remains just as complex as before,\n" +
              "only the user can no longer see it."
          )
        )
      )



      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          DumbTab(
            [
              Item(Id(:address), "&Address"),
              Item(Id(:overview), "&Overview"),
              Item(Id(:style), "GUI &Style Hints", true)
            ],
            Left(
              Top(
                HVSquash(
                  VBox(
                    VSpacing(0.3),
                    HBox(
                      HSpacing(1),
                      ReplacePoint(Id(:tabContents), @style_hints_page)
                    )
                  )
                )
              )
            )
          ), # true: selected
          Right(PushButton(Id(:close), "Cl&ose"))
        )
      )


      while true
        @widget = Convert.to_symbol(UI.UserInput)

        if @widget == :close
          break
        elsif @widget == :address
          UI.ReplaceWidget(:tabContents, @address_page)
        elsif @widget == :overview
          UI.ReplaceWidget(:tabContents, @overview_page)
        elsif @widget == :style
          UI.ReplaceWidget(:tabContents, @style_hints_page)
        end
      end

      UI.CloseDialog

      nil
    end
  end
end

Yast::DumbTab2Client.new.main
