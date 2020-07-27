sequence: main.o lcs.o lts.o lps.o util.o
	gcc -Wall -std=c99 main.o lcs.o lts.o lps.o util.o -o sequence

main.o: main.c main.h
	gcc -Wall -std=c99 -c main.c

lcs.o: lcs.c lcs.h
	gcc -Wall -std=c99 -c lcs.c

lts.o: lts.c lts.h
	gcc -Wall -std=c99 -c lts.c

lps.o: lps.c lps.h
	gcc -Wall -std=c99 -c lps.c

util.o: util.c util.h
	gcc -Wall -std=c99 -c util.c

clean:
	rm -f *.out *.o

timingscript: timingscript.c
	gcc -Wall -std=c99 timingscript.c -o timingscript
