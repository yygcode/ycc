SUBDIRS = common algos net . tests tools

lib_LTLIBRARIES = libycc.la
libycc_la_SOURCES =
libycc_la_LIBADD = \
	common/libycc-common.la \
	algos/libycc-algos.la

libycc_la_LDFLAGS = -release $(PACKAGE_VERSION) -version-info 1

EXTRA_DIST = $(srcdir)/include
