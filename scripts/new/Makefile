all:
	cc -o html html.c parser.c
	cc -DTESTING -o cparse parser.c

test:
	cc -DTESTING -o cparse parser.c

html:
	cc -o html html.c parser.c

clean:
	rm cparse html