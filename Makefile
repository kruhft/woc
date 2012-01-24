TARGET=woc
CC=gcc
CFLAGS=-Wall -g
INSTALLDIR=../../bin

all: $(TARGET)

$(TARGET): $(TARGET).o

install:
	cp -av $(TARGET) $(INSTALLDIR)
clean:
	rm -f *.o *~ $(TARGET)
