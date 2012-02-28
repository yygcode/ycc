include $(top_srcdir)/Makefile.rules
include $(top_srcdir)/src/tests/Makefile.rules

tests_common_dir = $(tests_dir)/common

noinst_PROGRAMS = test-debug test-ndebug test-log test-conf test-unistd

test_ndebug_SOURCES =
test_ndebug_LDADD = test-ndebug.o
test-ndebug.o: $(tests_common_dir)/test-debug.c $(top_builddir)/config.h
	$(COMPILE) -DYCC_TEST_NDEBUG -o test-ndebug.o -c \
		$(tests_common_dir)/test-debug.c
