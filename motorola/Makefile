MOT-SRCS = 68xxx ppc

all: mot-history.ps

mot-history.ps: mot-history
	dot -Tps mot-history >mot-history.ps

mot-history: $(MOT-SRCS)
	perl -w ../scripts/todot $(MOT-SRCS) >mot-history

clean:
	rm -f *.html *.css mot-history *.aux *.log mot-history.ps

