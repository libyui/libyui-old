# encoding: utf-8

# stupid puzzle
# $Id$
module Yast
  class PuzzleClient < Client
    def main
      Yast.import "UI"
      @line_len = 4


      # ==== main

      # define this global variable to provide nxn puzzle table

      # create buttons with space at num
      @buttons = generate_buttons(5)

      # play puzzle
      PuzzleWindow(@buttons)

      nil
    end

    def DebugDialog(str)
      str = deep_copy(str)
      UI.OpenDialog(
        VBox(RichText(Builtins.sformat("%1", str)), PushButton("&OK"))
      )
      r = UI.UserInput
      UI.CloseDialog

      nil
    end


    def buttons_table(buttons)
      buttons = deep_copy(buttons)
      tabl = VBox()
      line = HBox()

      i = 0
      j = 0
      label = ""

      while Ops.less_than(
          Ops.subtract(Ops.add(i, @line_len), 1),
          Builtins.size(buttons)
        )
        j = 0
        line = HBox()

        while Ops.less_than(j, @line_len)
          label = Ops.get(buttons, Ops.add(i, j), "")

          if Ops.greater_than(Builtins.size(label), 0)
            line = Builtins.add(
              line,
              ReplacePoint(
                Id(Builtins.sformat("rp%1", Ops.add(i, j))),
                PushButton(Id(Ops.add(i, j)), label)
              )
            )
          else
            line = Builtins.add(
              line,
              ReplacePoint(
                Id(Builtins.sformat("rp%1", Ops.add(i, j))),
                HStretch()
              )
            )
          end

          j = Ops.add(j, 1)
        end

        tabl = Builtins.add(tabl, line)

        i = Ops.add(i, j)
      end

      deep_copy(tabl)
    end


    def isRight(buttons, pos)
      buttons = deep_copy(buttons)
      if Ops.less_than(Ops.add(pos, 1), Builtins.size(buttons))
        return Ops.get_string(buttons, Ops.add(pos, 1), "") == ""
      end
      false
    end


    def isLeft(buttons, pos)
      buttons = deep_copy(buttons)
      if Ops.greater_or_equal(Ops.subtract(pos, 1), 0)
        return Ops.get_string(buttons, Ops.subtract(pos, 1), "") == ""
      end
      false
    end


    def isDown(buttons, pos)
      buttons = deep_copy(buttons)
      if Ops.less_than(Ops.add(pos, @line_len), Builtins.size(buttons))
        return Ops.get_string(buttons, Ops.add(pos, @line_len), "") == ""
      end
      false
    end


    def isUp(buttons, pos)
      buttons = deep_copy(buttons)
      if Ops.greater_or_equal(Ops.subtract(pos, @line_len), 0)
        return Ops.get_string(buttons, Ops.subtract(pos, @line_len), "") == ""
      end
      false
    end


    # do the move
    def move(buttons, pos, m)
      buttons = deep_copy(buttons)
      ret = []
      i = 0

      while Ops.less_than(i, Builtins.size(buttons))
        if i == pos
          Ops.set(ret, Builtins.size(ret), Ops.get(buttons, Ops.add(i, m)))
        elsif i == Ops.add(pos, m)
          Ops.set(ret, Builtins.size(ret), Ops.get(buttons, Ops.subtract(i, m)))
        else
          Ops.set(ret, Builtins.size(ret), Ops.get(buttons, i))
        end

        i = Ops.add(i, 1)
      end

      UI.ReplaceWidget(Id(Builtins.sformat("rp%1", pos)), HStretch())
      UI.ReplaceWidget(
        Id(Builtins.sformat("rp%1", Ops.add(pos, m))),
        PushButton(Id(Ops.add(pos, m)), Ops.get(buttons, pos))
      )


      deep_copy(ret)
    end


    # generate list of buttons
    # TODO: needs to be randomized
    def generate_buttons(space)
      buttons = []
      i = 1

      while Ops.less_or_equal(i, Ops.multiply(@line_len, @line_len))
        if i == space
          Ops.set(buttons, Builtins.size(buttons), "")
        else
          if Ops.less_than(i, space)
            Ops.set(buttons, Builtins.size(buttons), Builtins.sformat("%1", i))
          else
            Ops.set(
              buttons,
              Builtins.size(buttons),
              Builtins.sformat("%1", Ops.subtract(i, 1))
            )
          end
        end

        i = Ops.add(i, 1)
      end

      deep_copy(buttons)
    end


    def PuzzleWindow(buttons)
      buttons = deep_copy(buttons)
      UI.OpenDialog(
        HBox(
          HSpacing(3),
          VBox(
            VSpacing(),
            Frame("Puzzle", buttons_table(buttons)),
            VSpacing(5),
            PushButton(Id(:quit), "&Quit")
          ),
          HSpacing()
        )
      )

      ui = nil
      begin
        ui = UI.UserInput

        if Ops.is_integer?(ui)
          if isLeft(buttons, Convert.to_integer(ui))
            buttons = move(buttons, Convert.to_integer(ui), -1)
          elsif isRight(buttons, Convert.to_integer(ui))
            buttons = move(buttons, Convert.to_integer(ui), 1)
          elsif isUp(buttons, Convert.to_integer(ui))
            buttons = move(
              buttons,
              Convert.to_integer(ui),
              Ops.unary_minus(@line_len)
            )
          elsif isDown(buttons, Convert.to_integer(ui))
            buttons = move(buttons, Convert.to_integer(ui), @line_len)
          end
        end
      end until ui == :cancel || ui == :quit

      UI.CloseDialog

      nil
    end
  end
end

Yast::PuzzleClient.new.main
