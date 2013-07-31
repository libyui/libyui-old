# encoding: utf-8

module Yast
  class InternalErrorClient < Client
    def main
      Yast.import "UI"
      # This will cause an internal error since UI::UserInput() is called without
      # any dialog being opened.

      while true
        @ret = UI.UserInput
        Builtins.y2milestone("UI::UserInput() returned %1", @ret)
      end

      nil
    end
  end
end

Yast::InternalErrorClient.new.main
