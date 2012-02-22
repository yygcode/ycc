include $(top_srcdir)/Makefile.rules
test_LDADD = ../../libycc.la

noinst_PROGRAMS = test-debug test-ndebug

test_ndebug_SOURCES =

test-ndebug.o: $(top_srcdir)/src/tests/common/test-debug.c
	$(COMPILE) -DYCC_TEST_NDEBUG -o test-ndebug.o -c \
		$(top_srcdir)/src/tests/common/test-debug.c

test_debug_LDADD = $(test_LDADD)
test_ndebug_LDADD = test-ndebug.o $(test_LDADD)
