IBM-SRCS = ibmpc

all: ibm-history.ps

ibm-history.ps: ibm-history
	dot -Tps ibm-history > ibm-history.ps

ibm-history: $(SRCS)
	perl -w ../scripts/todot $(IBM-SRCS) > ibm-history

clean:
	rm -f *.html *.css ibm-history *.aux *.log ibm-history.ps








