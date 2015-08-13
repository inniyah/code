main: main.o trace.o
	gcc main.o trace.o -o main

main.o: main.c
	gcc -finstrument-functions -g -c -o main.o main.c

trace.o: trace.c
	gcc -c -o trace.o trace.c

test: main
	./main
	./readtracelog.sh main trace.out

clean:
	rm -fv *~ *.o main trace.out
