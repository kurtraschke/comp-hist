UNIX-SRCS = unix/2bsd unix/386bsd unix/4bsd unix/freebsd unix/netbsd\
	 unix/openbsd unix/research unix/usdl unix/sysV unix/bsdi\
	 unix/consensys unix/tandem unix/unixware unix/sunos\
	 unix/aos unix/minix unix/linux0 unix/darwin unix/linux1\
	 unix/mach unix/xenix unix/nextstep unix/linux2 unix/solaris\
	 unix/hurd unix/plan9 unix/osf unix/aix unix/hpux $(LINUX-SRCS)\
	 unix/mklinux unix/usg unix/cb unix/wollongong unix/lsx\
	 $(UNIX-STANDARDS) unix/corps unix/ultrix unix/sco unix/chorus\
	 unix/irix unix/digital unix/xinu unix/interactive $(SH-SRCS)

CPM-SRCS = cpm/windows cpm/cpm cpm/msdos cpm/winnt cpm/msos2\
	 cpm/wince cpm/ibmos2

CRAY-SRCS = cray/cray cray/corp

ORD-SRCS = ord-corps $(IBM-SRCS)

PARC-SRCS = parc

APPLE-SRCS = apple/applex apple/lisa apple/mac68k apple/osx apple/aux\
	 apple/macos apple/corp apple/servers apple/newton

NEXT-SRCS = next/corp next/hw

INTEL-SRCS = intel/4004 intel/pentium intel/corps intel/amd

IBM-SRCS = intel/pc/ibmpc $(POWER-SRCS) ibm/602 ibm/corp\
	 ibm/sys-360/360 $(CTSS-SRCS)

SINCLAIR-SRCS = sinclair

TRANSISTOR-SRCS = transistor $(DEC-SRCS)

CTSS-SRCS = ctss/ctss ctss/multics ctss/its $(UNIX-SRCS)

DEC-SRCS = dec/pdp dec/tops dec/rsts dec/pdp11 dec/corp dec/rsx\
	 dec/vms $(CPM-SRCS) $(WAITS-SRCS)

LINC-SRCS = linc $(PARC-SRCS)

DATAGEN-SRCS = datagen/nova datagen/corp

MOTOROLA-SRCS = motorola/68xxx motorola/ppc $(APPLE-SRCS)\
	 $(BE-SRCS) $(NEXT-SRCS)

BCPL-SRCS = bcpl/bcpl bcpl/c++ bcpl/obj-c

LISP-SRCS = lisp

SH-SRCS = sh/sh sh/awk sh/perl

BE-SRCS = be/corp be/bebox be/beos

LINUX-SRCS = unix/linux-dists/redhat unix/linux-dists/debian\
	 unix/linux-dists/leetnux unix/linux-dists/minilinux\
	 unix/linux-dists/mandrake

POWER-SRCS = ibm/power/power

UNIX-STANDARDS = unix/standards/usr-group unix/standards/posix

CPL-SRCS = bcpl/cpl $(BCPL-SRCS)

ALGOL-SRCS = algol/algol $(CPL-SRCS)

CORP-SRCS = fairchildsemi corps

WAITS-SRCS = waits

FORTRAN-SRCS = fortran/fortran

RFC-STDS = rfc/off-stds rfc/assgin-nums rfc/host-req-comm\
	 rfc/host-req-apps

PALM-SRCS = palm/palm palm/palmos palm/pcpdev

SUN-SRCS = sun/corp

SGI-SRCS = sgi/corp

SRCS = $(ORD-SRCS) $(INTEL-SRCS) $(SINCLAIR-SRCS) $(LINC-SRCS)\
	 $(DATAGEN-SRCS) $(TRANSISTOR-SRCS) $(MOTOROLA-SRCS)\
	 $(LISP-SRCS) $(ALGOL-SRCS) $(CRAY-SRCS) $(CORP-SRCS)\
	 $(FORTRAN-SRCS) $(RFC-SRCS) $(PALM-SRCS) $(SUN-SRCS)\
	 $(SGI-SRCS)

all: dump vcg biblio info html chml browser dot dbm

dot: dump
	perl -w scripts/parsech-split/dot dump >comp-history.dot

dot.ps: dot
	dot -Tps comp-history.dot >comp-history.dot.ps

clean:
	rm -rf *.html *.css comp-history* *.aux *.log *.ps bibliography *.pdf information dump *xml pod2html-* parsech_dbm/
	@echo "That's it for the normal cleanup. However, if you've built the browser, you probably want to cd to scripts/browser, and make clean there too."

biblio: dump
	perl scripts/parsech-split/biblio dump > bibliography

info: dump
	perl scripts/parsech-split/info dump > information

vcg: dump
	perl -w scripts/parsech-split/vcg dump >comp-history.vcg 

vcg.ps: vcg
	xvcg comp-history.vcg -psoutput comp-history.vcg

html: dump
	perl scripts/parsech-split/html dump >comp-hist.html

chml: dump
	perl scripts/parsech-split/chml dump >comp-hist.xml

browser:
	cd scripts/browser ; ./configure ; make 

browse: browser dump
	cd scripts/browser/src ; ./browser ../../../dump

dbm: dump
	perl scripts/parsech-split/dbm dump

$(SRCS) :

dump: $(SRCS)
	cat $(SRCS) | unexpand > dump

sources:
	@echo $(SRCS)