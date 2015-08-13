all: sender receiver

CFLAGS= -O2 -g -Wall
LDFLAGS= -Wl,-z,defs -Wl,--as-needed -Wl,--no-undefined
DEFS = 

INCS = 
LIBS = 

sender: sender.o
	g++ $(LDFLAGS) $+ -o $@ $(LIBS)

receiver: receiver.o
	g++ $(LDFLAGS) $+ -o $@ $(LIBS)

%.o: %.cpp
	g++ -o $@ -c $+ $(DEFS) $(INCS) $(CFLAGS)

%.o: %.c
	gcc -o $@ -c $+ $(DEFS) $(INCS) $(CFLAGS)

clean:
	rm -fv sender receiver *.o *~
