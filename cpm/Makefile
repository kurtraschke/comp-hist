CPM-SRCS = windows cpm msdos

all: cpm-history.ps

cpm-history.ps: cpm-history
	dot -Tps cpm-history >cpm-history.ps

cpm-history: $(CPM-SRCS)
	perl -w ../scripts/todot $(CPM-SRCS) >cpm-history

clean:
	rm -f *.html *.css cpm-history *.aux *.log cpm-history.ps

