UNIX-SRCS = unix/2bsd unix/386bsd unix/4bsd unix/freebsd unix/netbsd unix/openbsd unix/research unix/usdl unix/sysV unix/bsdi unix/consensys unix/tandem unix/unixware unix/sunos unix/aos unix/minix unix/linux0 unix/darwin unix/linux1

CPM-SRCS = cpm/windows cpm/cpm cpm/msdos

ORD-SRCS = ord-corps

PARC-SRCS = parc

APPLE-SRCS = apple/applex apple/lisa apple/mac68k apple/osx apple/aux apple/macos apple/corp apple/ans

INTEL-SRCS = intel/4004 intel/pentium intel/corps intel/amd

IBM-SRCS = intel/pc/ibmpc 

SINCLAIR-SRCS = sinclair

TRANSISTOR-SRCS = transistor

CTSS-SRCS = ctss/ctss ctss/multics $(UNIX-SRCS)

DEC-SRCS = dec/pdp dec/tops dec/rsts dec/pdp11 dec/corp

LINC-SRCS = linc

DATAGEN-SRCS = datagen/nova datagen/corp

MOTOROLA-SRCS = motorola/68xxx motorola/ppc

SRCS = $(CTSS-SRCS) $(CPM-SRCS) $(ORD-SRCS) $(PARC-SRCS) $(APPLE-SRCS) $(INTEL-SRCS) $(IBM-SRCS) $(SINCLAIR-SRCS) $(DEC-SRCS) $(LINC-SRCS) $(DATAGEN-SRCS) $(TRANSISTOR-SRCS) $(MOTOROLA-SRCS)

all: comp-history-vcg biblio info

comp-history-dot: 
	perl -w scripts/todot $(SRCS) >comp-history.dot

comp-history-dot.ps: comp-history-dot
	dot -Tps comp-history.dot >comp-history.dot.ps
clean:
	rm -f *.html *.css comp-history* *.aux *.log /tmp/biblio bibliography *.pdf information

biblio:
	cat $(SRCS) | scripts/biblio > bibliography

info:
	cat $(SRCS) | sh ./scripts/info > information

comp-history-vcg:
	perl -w scripts/tovcg.exp $(SRCS) >comp-history.vcg 

comp-history-vcg.ps: comp-history-vcg
	xvcg comp-history.vcg -psoutput comp-history.vcg.ps

comp-history-gp:
	perl -w scripts/togp $(SRCS) >comp-history.gp

comp-history-gp.ps: comp-history-gp
	graphplace -p -a comp-history.gp >comp-history.gp.ps


