INTEL-SRCS = 4004

all: intel-history.ps

intel-history.ps: intel-history
	dot -Tps intel-history > intel-history.ps

intel-history: $(SRCS)
	perl -w ../scripts/todot $(INTEL-SRCS) > intel-history

clean:
	rm -f *.html *.css intel-history *.aux *.log intel-history.ps








