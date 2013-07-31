# encoding: utf-8

module Yast
  class Weight2Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        VBox(
          HBox(
            HWeight(1, PushButton(Opt(:hstretch), "Weight 1")),
            HWeight(1, PushButton(Opt(:hstretch), "Weight 1"))
          ),
          HBox(
            HWeight(3, PushButton(Opt(:hstretch), "Weight 3")),
            HWeight(2, PushButton(Opt(:hstretch), "Weight 2"))
          ),
          HBox(
            HWeight(2, PushButton(Opt(:hstretch), "Weight 2")),
            HWeight(1, PushButton(Opt(:hstretch), "Weight 1"))
          ),
          HBox(
            HWeight(3, PushButton(Opt(:hstretch), "Weight 3")),
            HWeight(1, PushButton(Opt(:hstretch), "Weight 1"))
          ),
          HBox(
            HWeight(10, PushButton(Opt(:hstretch), "Weight 10")),
            HWeight(1, PushButton(Opt(:hstretch), "Weight 1"))
          )
        )
      )
      UI.UserInput
      UI.CloseDialog

      nil
    end
  end
end

Yast::Weight2Client.new.main
