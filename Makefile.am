#
# Makefile.am for libyui
#

SUBDIRS = src cmake examples 

AUTOMAKE_OPTIONS = foreign dist-bzip2 no-dist-gzip

libyui.pc: libyui.pc.in config.status
        ./config.status --file=$@:$<

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libyui.pc

EXTRA_DIST = libyui.pc.in

$(RPMNAME)-$(VERSION).tar.bz2: dist-bzip2

package-local: $(RPMNAME)-$(VERSION).tar.bz2
	rm -f package/$(RPMNAME)-*.tar.bz2
	rm -f package/*~
	rm -f package/*.bak
	mv $(RPMNAME)-$(VERSION).tar.bz2 package/

package: package-local
