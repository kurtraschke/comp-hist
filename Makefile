UNIX-SRCS = unix/2bsd unix/386bsd unix/4bsd unix/freebsd unix/netbsd unix/openbsd unix/research unix/usdl unix/sysV unix/bsdi unix/consensys unix/tandem unix/unixware unix/sunos unix/aos unix/minix unix/linux

CPM-SRCS = cpm/windows cpm/cpm

all: comp-history.ps

comp-history: 
	perl -w scripts/todot $(CPM-SRCS) $(UNIX-SRCS) >comp-history

comp-history.ps: comp-history 
	dot -Tps comp-history >comp-history.ps

unix-history.ps: unix-history
	dot -Tps unix/unix-history

unix-history: $(SRCS)
	perl -w scripts/todot $(UNIX-SRCS)

clean:
	rm -f *.html *.css comp-history *.aux *.log comp-history.ps

