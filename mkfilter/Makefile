PROGRAMS=mkfilter mkshape mkaverage gencode genplot

all: $(PROGRAMS)

CFLAGS= -O2 -g -Wall
LDFLAGS= -Wl,-z,defs -Wl,--as-needed -Wl,--no-undefined

mkfilter: mkfilter.o complex.o
	g++ $(LDFLAGS) $+ -o $@ -lm

mkshape: mkshape.o complex.o
	g++ $(LDFLAGS) $+ -o $@ -lm

mkaverage: mkaverage.o complex.o
	g++ $(LDFLAGS) $+ -o $@ -lm

gencode: gencode.o complex.o readdata.o
	g++ $(LDFLAGS) $+ -o $@ -lm

genplot: genplot.o complex.o readdata.o
	g++ $(LDFLAGS) $+ -o $@ -lgd -lm

%.o: %.cpp
	g++ -o $@ -c $+ $(CFLAGS) $(PKG_CONFIG_CFLAGS)

%.o: %.c
	gcc -o $@ -c $+ $(CFLAGS) $(PKG_CONFIG_CFLAGS)

clean:
	rm -fv $(PROGRAMS)
	rm -fv *.o *~

