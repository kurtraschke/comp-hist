UNIX-SRCS= 2bsd 386bsd 4bsd freebsd netbsd openbsd research usdl sysV bsdi consensys tandem unixware sunos aos minix linux0 darwin linux1

all: unix-history.ps

unix-history.ps: unix-history
	dot -Tps unix-history > unix-history.ps

unix-history: $(SRCS)
	perl -w ../scripts/todot $(UNIX-SRCS) > unix-history

clean:
	rm -f *.html *.css unix-history *.aux *.log unix-history.ps








