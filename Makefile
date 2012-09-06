CC= gcc
VERSION = "0.1.4"
CFLAGS = -g -O2
LIBS =  
LDFLAGS = 

all: wormulon-bin

wormulon-bin:	wormulon.c
			$(CC) $(CFLAGS) -DVERSION='$(VERSION)' wormulon.c $(LIBS) -o wormulon
prof:	wormulon.c
			$(CC) $(CFLAGS) -DVERSION='$(VERSION)' wormulon.c $(LIBS) -o wormulon -g -pg

clean:
	rm -f wormulon wormulon.o src/*.o

distclean:
		rm -rf wormulon wormulon.o src/*.o config.h Makefile configure.scan configure.in.orig  config.h config.log config.status autom4te.cache
