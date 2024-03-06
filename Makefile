IDIR=./
COMPILER=nvcc
SRCDIR=src
APPNAME=nppresize
OUTPUTDIR=bin
LIBRARIES= -lnppisu_static -lnppif_static -lnppc_static -lculibos -lfreeimage -lcudart -lcuda -lnppig -lnppicc -lnppisu

COMPILER_FLAGS=-I$(IDIR) -I/usr/local/cuda/include -I/usr/local/cuda/lib64  -I./Common/UtilNPP -I./Common  $(LIBRARIES) --std c++17

.PHONY: clean build run

build:
	$(COMPILER) $(COMPILER_FLAGS) src/*.cpp -o $(OUTPUTDIR)/$(APPNAME).exe

clean:
	rm -f $(OUTPUTDIR)/$(APPNAME).exe

run:
	$(OUTPUTDIR)/$(APPNAME).exe --input data/Lena.pgm --output Lena_sacaled.pgm --scale 2

all: clean build run