include Makefile.rules

SUBDIRS = src man

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = AUTHORS BUGS ChangeLog COPYING Makefile.rules NEWS README

install-dev: install
	$(MKDIR_P) "$(DESTDIR)$(includedir)" || exit $?
	cp -av $(top_srcdir)/$(ycc_headerdir)/ycc "$(DESTDIR)$(includedir)" \
		|| exit $?
