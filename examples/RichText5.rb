# encoding: utf-8

# Example for a RichText widget
module Yast
  class RichText5Client < Client
    def main
      Yast.import "UI"
      UI.OpenDialog(
        Opt(:defaultsize),
        VBox(
          RichText(
            "<pre>\t\t\t\t--- /.snapshots/4/snapshot/var/lib/zypp/RequestedLocales\t2011-12-08 17:09:23.312007703 +0800<br><br>+++ /.snapshots/5/snapshot/var/lib/zypp/RequestedLocales\t2011-12-08 18:39:35.461081330 +0800<br>@@ -1,4 +1,4 @@<br>-# zypp::RequestedLocales generated Thu 08 Dec 2011 05:09:23 PM WST<br>+# zypp::RequestedLocales generated Do 08\tDez\t2011\t18:39:35\tWST<br> #<br>-de_DE<br>+en_US<br> #<br></pre><p>Formfeed test:Here it should be ignored.</p><pre>After the next wordshould be a newline.</pre>"
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

Yast::RichText5Client.new.main
