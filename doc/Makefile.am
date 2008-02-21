#
# Makefile.am for core/libyui/doc
#

SUBDIRS = events examples FAQs

htmldir = $(docdir)/libyui
builtin	= $(srcdir)/../src/ycp-ui/YCP_UI.cc $(srcdir)/../src/ycp-ui/YCPDialogParser.cc
builtinxml := $(builtin:%.cc=%.xml)
xmlfiles := $(foreach xml,$(builtinxml),$(notdir $(xml)))
htmlfiles = $(wildcard html/*.html)

html_DATA =			\
	ui-arch.jpg		\
	README.newapi		\
	html/index.html		\
	$(htmlfiles)		\
	html/yast2docs.css

EXTRA_DIST = ui-arch.jpg \
	     README.newapi \
	     YCP-UI-layout.xml \
	     YCP-UI-richtext.xml \
	     macro-recorder.xml \
	     ui.xml



$(xmlfiles) : $(builtin)
	`pkg-config --variable=ybindir yast2-devtools`/ydoc2 -o $@.tmp ../src/ycp-ui/$(subst xml,cc,$@)
	$(XSLTPROC) $(XSLTPROC_FLAGS) @STYLESHEET_YDOC@ $@.tmp > $@
	`pkg-config --variable=ybindir yast2-devtools`/replace_entities $@
	rm $@.tmp

html/index.html: $(xmlfiles) ui.xml
	XML_CATALOG_FILES=@XML_CATALOG@ $(XSLTPROC) $(XSLTPROC_FLAGS) --xinclude @STYLESHEET_HTML@ ui.xml

html/yast2docs.css: html/index.html
	cp @STYLESHEET_CSS@ html
	cp -a `pkg-config --variable=yast2dir yast2-devtools`/data/docbook/images html
	cp -a $(srcdir)/examples html
	-find html -name .svn -type d -exec rm -rf '{}' \;

CLEANFILES = $(xmlfiles) output.xml

clean-local:
	rm -rf html

install-data-local:
	mkdir -p $(DESTDIR)$(htmldir)
	cp -a $(srcdir)/html/images $(DESTDIR)$(htmldir)
	cp -a $(srcdir)/examples $(DESTDIR)$(htmldir)
	-find $(DESTDIR)$(htmldir) -name .svn -type d -exec rm -rf '{}' \;
