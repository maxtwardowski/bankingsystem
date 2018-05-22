all: bankingsystem

bankingsystem: bankingsystem.o stringtools.o dbtools.o
	gcc -g $^ -o $@ -lm -std="c99"

.c.o:
	gcc -g -std=c99 -c -D_REENTRANT $<

stringtools.o: stringtools.c stringtools.h

dbtools.o: dbtools.c dbtools.h

bankingsystem.o: bankingsystem.c stringtools.h dbtools.h

.PHONY: clean

clean:
	-rm bankingsystem

run:
	./bankingsystem

fullclean:
	-rm bankingsystem database.dat bankingsystem.o stringtools.o dbtools.o
