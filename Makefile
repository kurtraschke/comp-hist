UNIX-SRCS = unix/2bsd unix/386bsd unix/4bsd unix/freebsd unix/netbsd unix/openbsd unix/research unix/usdl unix/sysV unix/bsdi unix/consensys unix/tandem unix/unixware unix/sunos unix/aos unix/minix unix/linux0 unix/darwin unix/linux1 unix/mach unix/xenix unix/nextstep unix/linux2 unix/solaris unix/hurd unix/plan9 unix/osf unix/aix unix/hpux $(LINUX-SRCS) unix/mklinux unix/usg unix/cb unix/wollongong unix/lsx $(UNIX-STANDARDS) unix/corps unix/ultrix unix/sco unix/chorus unix/irix unix/digital unix/xinu unix/interactive

CPM-SRCS = cpm/windows cpm/cpm cpm/msdos cpm/winnt

CRAY-SRCS = cray/cray cray/corp

ORD-SRCS = ord-corps

PARC-SRCS = parc

APPLE-SRCS = apple/applex apple/lisa apple/mac68k apple/osx apple/aux apple/macos apple/corp apple/servers apple/newton

NEXT-SRCS = next/corp next/hw

INTEL-SRCS = intel/4004 intel/pentium intel/corps intel/amd

IBM-SRCS = intel/pc/ibmpc $(POWER-SRCS) ibm/602 ibm/corp

SINCLAIR-SRCS = sinclair

TRANSISTOR-SRCS = transistor

CTSS-SRCS = ctss/ctss ctss/multics ctss/its $(UNIX-SRCS)

DEC-SRCS = dec/pdp dec/tops dec/rsts dec/pdp11 dec/corp dec/rsx dec/vms

LINC-SRCS = linc

DATAGEN-SRCS = datagen/nova datagen/corp

MOTOROLA-SRCS = motorola/68xxx motorola/ppc

BCPL-SRCS = bcpl/bcpl bcpl/c++ bcpl/obj-c

LISP-SRCS = lisp

SH-SRCS = sh/sh sh/awk sh/perl

BE-SRCS = be/corp be/bebox be/beos

LINUX-SRCS = unix/linux-dists/redhat unix/linux-dists/debian unix/linux-dists/leetnux unix/linux-dists/minilinux

POWER-SRCS = ibm/power/power

UNIX-STANDARDS = unix/standards/usr-group unix/standards/posix

CPL-SRCS = bcpl/cpl $(BCPL-SRCS)

ALGOL-SRCS = algol/algol $(CPL-SRCS)

CORP-SRCS = fairchildsemi corps

SRCS = $(CTSS-SRCS) $(CPM-SRCS) $(ORD-SRCS) $(PARC-SRCS) $(APPLE-SRCS) $(INTEL-SRCS) $(IBM-SRCS) $(SINCLAIR-SRCS) $(DEC-SRCS) $(LINC-SRCS) $(DATAGEN-SRCS) $(TRANSISTOR-SRCS) $(MOTOROLA-SRCS) $(NEXT-SRCS) $(LISP-SRCS) $(SH-SRCS) $(BE-SRCS) $(ALGOL-SRCS) $(CRAY-SRCS) $(CORP-SRCS)

all: comp-history-vcg biblio info

comp-history-dot: 
	perl -w scripts/todot $(SRCS) >comp-history.dot

comp-history-dot.ps: comp-history-dot
	dot -Tps comp-history.dot >comp-history.dot.ps
clean:
	rm -f *.html *.css comp-history* *.aux *.log *.ps bibliography *.pdf information dump

biblio:
	cat $(SRCS) | scripts/biblio > bibliography

info:
	perl -w scripts/info $(SRCS) > information

comp-history-vcg:
	perl -w scripts/tovcg $(SRCS) >comp-history.vcg 

comp-history-vcg.ps: comp-history-vcg
	xvcg comp-history.vcg -psoutput comp-history.vcg.ps

comp-history-gp:
	perl -w scripts/togp $(SRCS) >comp-history.gp

comp-history-gp.ps: comp-history-gp
	graphplace -p -a comp-history.gp >comp-history.gp.ps


dump:
	cat $(SRCS) >dump
