PROGRAM=fftn

all: $(PROGRAM)

SOURCES = main.cpp

HEADERS = fftn.h
OBJS = $(SOURCES:.cpp=.o)

PKG_CONFIG=
PKG_CONFIG_CFLAGS=`pkg-config --cflags $(PKG_CONFIG) 2>/dev/null`
PKG_CONFIG_LIBS=`pkg-config --libs $(PKG_CONFIG) 2>/dev/null`

CFLAGS= -O2 -g -Wall

LDFLAGS= -Wl,-z,defs -Wl,--as-needed -Wl,--no-undefined
LIBS=$(PKG_CONFIG_LIBS) -lsfml-window -lsfml-network -lsfml-graphics -lsfml-audio -lsfml-system

$(PROGRAM): $(OBJS)
	g++ $(LDFLAGS) $+ -o $@ $(LIBS)

%.o: %.cpp $(HEADERS)
	g++ -o $@ -c $< $(CFLAGS) $(PKG_CONFIG_CFLAGS)

%.o: %.c $(HEADERS)
	gcc -o $@ -c $< $(CFLAGS) $(PKG_CONFIG_CFLAGS)

clean:
	rm -fv $(OBJS)
	rm -fv $(PROGRAM)
	rm -fv *~

