UNIX-SRCS = unix/2bsd unix/386bsd unix/4bsd unix/freebsd unix/netbsd unix/openbsd unix/research unix/usdl unix/sysV unix/bsdi unix/consensys unix/tandem unix/unixware unix/sunos unix/aos unix/minix unix/linux unix/darwin

CPM-SRCS = cpm/windows cpm/cpm

ORD-SRCS = ord-corps

PARC-SRCS = parc

APPLE-SRCS = apple/applex apple/lisa apple/mac68k apple/macppc apple/osx

INTEL-SRCS = intel/4004 intel/pentium

IBM-SRCS = ibmpc 

SRCS = $(UNIX-SRCS) $(CPM-SRCS) $(ORD-SRCS) $(PARC-SRCS) $(APPLE-SRCS) $(INTEL-SRCS) $(IBM-SRCS)

all: comp-history.ps

comp-history: 
	perl -w scripts/todot $(SRCS) >comp-history

comp-history.ps: comp-history 
	dot -Tps comp-history >comp-history.ps

unix-history.ps: unix-history
	dot -Tps unix/unix-history

unix-history:
	perl -w scripts/todot $(UNIX-SRCS)

#ord-history.ps: ord-history
#        dot -Tps ord-corps-history

#ord-history:
#        perl -w scripts/todot $(ORD-SRCS)

#parc-history.ps: parc-history
#        dot -Tps parc-history

#parc-history: $(SRCS)
#        perl -w scripts/todot $(PARC-SRCS)

clean:
	rm -f *.html *.css comp-history *.aux *.log comp-history.ps

