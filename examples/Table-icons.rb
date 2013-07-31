# encoding: utf-8

# Advanced table example: Icon browser
module Yast
  class TableIconsClient < Client
    def main
      Yast.import "UI"



      @iconBasePath = "/usr/share/YaST2/theme/current/icons"

      UI.OpenDialog(
        VBox(
          Heading("Icons"),
          MinSize(
            60,
            40,
            Table(Id(:iconTable), Header("Name", "22x22", "32x32", "48x48"))
          ),
          HBox(
            Label(Opt(:outputField, :hstretch), @iconBasePath),
            PushButton(Id(:cancel), "&Close")
          )
        )
      )

      UI.OpenDialog(Label("Reading icon directories..."))


      @iconList = readIconDir(Ops.add(@iconBasePath, "/22x22/apps"))
      @iconList = Convert.convert(
        Builtins.union(
          @iconList,
          readIconDir(Ops.add(@iconBasePath, "/32x32/apps"))
        ),
        :from => "list",
        :to   => "list <string>"
      )
      @iconList = Convert.convert(
        Builtins.union(
          @iconList,
          readIconDir(Ops.add(@iconBasePath, "/48x48/apps"))
        ),
        :from => "list",
        :to   => "list <string>"
      )

      @itemList = []

      Builtins.foreach(@iconList) do |iconName|
        item = Item(
          Id(iconName),
          iconName,
          term(:cell, term(:icon, Ops.add("22x22/apps/", iconName))),
          term(:cell, term(:icon, Ops.add("32x32/apps/", iconName))),
          term(:cell, term(:icon, Ops.add("48x48/apps/", iconName)))
        )
        # y2debug( "Item: %1", item );
        @itemList = Builtins.add(@itemList, item)
      end

      UI.CloseDialog # Close busy dialog

      UI.ChangeWidget(:iconTable, :IconPath, @iconBasePath)
      UI.ChangeWidget(:iconTable, :Items, @itemList)

      @widgetID = nil
      begin
        @widgetID = UI.UserInput
      end until @widgetID == :cancel

      UI.CloseDialog

      nil
    end

    # Read a directory with icons.
    #
    def readIconDir(dir)
      iconList = Convert.convert(
        SCR.Read(path(".target.dir"), dir),
        :from => "any",
        :to   => "list <string>"
      )
      Builtins.y2debug("Dir %1: %2  entries", dir, Builtins.size(iconList))
      iconList = Builtins.sort(Builtins.filter(iconList) do |entry|
        Builtins.regexpmatch(entry, "^.*.(png|jpg|PNG|JPG)$")
      end)

      deep_copy(iconList)
    end
  end
end

Yast::TableIconsClient.new.main
