## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPARSER -I$(top_srcdir)/intl \
	@GTK_CFLAGS@

bin_PROGRAMS = browser

browser_SOURCES = \
	main.c \
	node.h parser.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h

browser_LDADD = @GTK_LIBS@ $(INTLLIBS)

