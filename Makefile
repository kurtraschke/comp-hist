UNIX-SRCS = unix/2bsd unix/386bsd unix/4bsd unix/freebsd unix/netbsd unix/openbsd unix/research unix/usdl unix/sysV unix/bsdi unix/consensys unix/tandem unix/unixware unix/sunos unix/aos unix/minix unix/linux0 unix/darwin unix/linux1

CPM-SRCS = cpm/windows cpm/cpm cpm/msdos

ORD-SRCS = ord-corps

PARC-SRCS = parc

APPLE-SRCS = apple/applex apple/lisa apple/mac68k apple/macppc apple/osx apple/aux apple/macos

INTEL-SRCS = intel/4004 intel/pentium

IBM-SRCS = intel/pc/ibmpc 

SINCLAIR-SRCS = sinclair

TRANSISTOR-SRCS = transistor

CTSS-SRCS = ctss/ctss $(UNIX-SRCS)

DEC-SRCS = dec/pdp dec/tops dec/rsts

LINC-SRCS = linc

DATAGEN-SRCS = datagen/nova

SRCS = $(CTSS-SRCS) $(CPM-SRCS) $(ORD-SRCS) $(PARC-SRCS) $(APPLE-SRCS) $(INTEL-SRCS) $(IBM-SRCS) $(SINCLAIR-SRCS) $(DEC-SRCS) $(LINC-SRCS) $(DATAGEN-SRCS) $(TRANSISTOR-SRCS)

all: comp-history-dot.ps biblio comp-history-vcg

comp-history-dot: 
	perl -w scripts/todot $(SRCS) >comp-history-dot

comp-history-dot.ps: comp-history-dot
	dot -Tps comp-history-dot >comp-history-dot.ps
clean:
	rm -f *.html *.css comp-history* *.aux *.log /tmp/biblio bibliography *.pdf

biblio:
	cat $(SRCS) >/tmp/biblio
	scripts/biblio
	rm /tmp/biblio

comp-history-vcg:
	perl -w scripts/tovcg $(SRCS) >comp-history.vcg 2>/dev/null
comp-history-vcg.ps: comp-history-vcg
	xvcg comp-history.vcg -psoutput comp-history.vcg.ps





