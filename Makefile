CFLAGS=-Wall -g

all: woc

woc: woc.o

clean:
	rm -f *.o *~ woc
