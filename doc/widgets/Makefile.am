# Makefile.am for core/libyui/doc/widgets

widgetdir = $(docdir)/libyui/widgets

widget_DATA = index.html $(wildcard *.html)
EXTRA_DIST = $(widget_DATA) make_widget_doc

# MAINTAINERCLEANFILES = $(widget_DATA)

CLEANFILES = $(widget_DATA)

$(filter-out index.html, $(widget_DATA)): index.html
index.html: $(srcdir)/../../src/*.cc
	$(srcdir)/make_widget_doc -s $(srcdir)/../../src

