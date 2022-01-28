.PHONY: test

TABLEFILE = all.tab
TC = B E J K N R S T

CFLAGS = -I.. -DTEST
LDFLAGS = -lm

all: nist-its90-B.c

nist-its90-B.c: nistcoeff.pl
	perl nistcoeff.pl $(TABLEFILE)
	chmod -w nist-its90-?.c nist-its90.h

rebuild:
	chmod +w nist-its90-?.c nist-its90.h
	$(MAKE)

test: test-inverse test-t2e

# check if emf2temp(temp2emf(T)) = T
test-inverse: build-test-bin
	@for i in $(TC); do \
		echo "  testing if emf2temp(temp2emf(T)) = T for Thermocouple $$i" ; \
		test/test-$$i ; \
	done

# check if emf2temp() generates the same data in NIST database
test-t2e: build-test-bin
	@for i in $(TC); do \
		echo "  making table test/test-$$i" ; \
		test/test-$$i -c > test/test-$$i-calc.dat ; \
	done
	perl nisttest.pl $(TABLEFILE)

build-test-bin: nist-its90-B.c nist-its90.o
	-mkdir test
	@for i in $(TC); do \
		echo "  building test/test-$$i" ; \
		$(CC) nist-its90-$$i.c nist-its90.o $(CFLAGS) -o test/test-$$i $(LDFLAGS) ; \
	done

clean:
	-rm -f *.o

distclean: clean
	-rm -rf test/

realclean: distclean
	-rm -f nist-its90-?.c nist-its90.h

nist-its90.o: nist-its90.c nist-its90.h
