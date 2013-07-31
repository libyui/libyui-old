# encoding: utf-8

module Yast
  class GlyphsClient < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          Heading("Glyphs"),
          Left(Label(Ops.add(UI.Glyph(:ArrowLeft), " ArrowLeft"))),
          Left(Label(Ops.add(UI.Glyph(:ArrowRight), " ArrowRight"))),
          Left(Label(Ops.add(UI.Glyph(:ArrowUp), " ArrowUp"))),
          Left(Label(Ops.add(UI.Glyph(:ArrowDown), " ArrowDown"))),
          Left(Label(Ops.add(UI.Glyph(:CheckMark), " CheckMark"))),
          Left(Label(Ops.add(UI.Glyph(:BulletArrowRight), " BulletArrowRight"))),
          Left(Label(Ops.add(UI.Glyph(:BulletCircle), " BulletCircle"))),
          Left(Label(Ops.add(UI.Glyph(:BulletSquare), " BulletSquare"))),
          VSpacing(),
          Left(
            Label(
              "If you see only question marks,\n" +
                "maybe you forgot to start this example\n" +
                "with the correct font -\n" +
                "use the 'start_glyphs' script!"
            )
          ),
          PushButton(Opt(:default), "&OK")
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::GlyphsClient.new.main
