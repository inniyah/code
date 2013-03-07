LDFLAGS= -Wl,-export-dynamic
CFLAGS=-finstrument-functions
LIBS=-ldl

test: test.o tracer.o main.o
	g++ $(LDFLAGS) $+ -o $@ $(LIBS)

%.o: %.cpp
	g++ -o $@ -c $+ $(CFLAGS)

%.o: %.c
	gcc -o $@ -c $+ $(CFLAGS)

clean:
	rm -f test
	rm -f *.o *~

