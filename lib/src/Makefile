# $Header$
# this Makefile creates a SwissEph library and a swetest sample on 64-bit
# Redhat Enterprise Linux RHEL 6.

# The mode marked as 'Linux' should also work with the GNU C compiler
# gcc on other systems.

# If you modify this makefile for another compiler, please
# let us know. We would like to add as many variations as possible.
# If you get warnings and error messages from your compiler, please
# let us know. We like to fix the source code so that it compiles
# free of warnings.
# send email to the Swiss Ephemeris mailing list.
#

CFLAGS = -g -Wall -fPIC -O2 -DUSECASE=2 # for Linux and other gcc systems
OP=$(CFLAGS)
CC=emcc	#for Linux

# compilation rule for general cases
.o :
	$(CC) $(OP) -o $@ $? -lm
.c.o:
	$(CC) -c $(OP) $<

SWEOBJ = swedate.o swehouse.o swejpl.o swemmoon.o swemplan.o sweph.o\
	 swephlib.o swecl.o swehel.o

astro: astro.o libswe.a
	$(CC) $(OP) --preload-file eph -o astro.js astro.o -s WASM=1 -s  EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' -L. -lswe -lm -ldl

install:
	# mv astro.js ../../js/; mv astro.wasm ../../js/; mv astro.wast ../../js/
	mv astro.js ../../js/; mv astro.wasm ../../js/; mv astro.data ../../js/

# create an archive and a dynamic link libary fro SwissEph
# a user of this library will inlcude swephexp.h  and link with -lswe

libswe.a: $(SWEOBJ)
	ar r libswe.a	$(SWEOBJ)

libswe.so: $(SWEOBJ)
	$(CC) -shared -o libswe.so $(SWEOBJ)

clean:
	rm -f *.o *.wasm *.wast *.js *.html astro libswe*; rm ../../js/astro.*

###
swecl.o: swejpl.h sweodef.h swephexp.h swedll.h sweph.h swephlib.h
sweclips.o: sweodef.h swephexp.h swedll.h
swedate.o: swephexp.h sweodef.h swedll.h
swehel.o: swephexp.h sweodef.h swedll.h
swehouse.o: swephexp.h sweodef.h swedll.h swephlib.h swehouse.h
swejpl.o: swephexp.h sweodef.h swedll.h sweph.h swejpl.h
swemini.o: swephexp.h sweodef.h swedll.h
swemmoon.o: swephexp.h sweodef.h swedll.h sweph.h swephlib.h
swemplan.o: swephexp.h sweodef.h swedll.h sweph.h swephlib.h swemptab.h
## swepcalc.o: swepcalc.h swephexp.h sweodef.h swedll.h
sweph.o: swejpl.h sweodef.h swephexp.h swedll.h sweph.h swephlib.h
swephlib.o: swephexp.h sweodef.h swedll.h sweph.h swephlib.h
## swetest.o: swephexp.h sweodef.h swedll.h
