APPLE-SRCS = applex lisa mac68k macppc osx

all: apple-history.ps

apple-history.ps: apple-history
	dot -Tps apple-history

apple-history:
	perl -w scripts/todot $(APPLE-SRCS) > apple-history

clean:
	rm -f *.html *.css apple-history *.aux *.log apple-history.ps