SRCS = `find ./src/ | sed /CVS.*/d`

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
