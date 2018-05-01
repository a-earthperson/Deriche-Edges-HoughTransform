IDIR =./include
CC=gcc
CFLAGS=-O2 -I $(IDIR) -Wall

MKDIR_P = mkdir -p
ODIR=./build
LDIR =./lib

LIBS=-lm

${ODIR}:
	${MKDIR_P} ${ODIR}

_DEPS = bmp.h deriche.h hough.h mat.h otsu.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o bmp.o mat.o hough.o deriche.o otsu.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: run
run: edgeDetect
	./edgeDetect

.PHONY: all
all: run

edgeDetect: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o examples/*.csv generated/* edgeDetect
