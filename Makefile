OBJ=main.o efunc.o
SRC=main.c efunc.c

CFLAGS=-O2 -g -Wall -Wwrite-strings

timecard : main.o efunc.o
	gcc -o $@ main.o efunc.o
main.o : efunc.h
efunc.o : efunc.h

clean :
	rm -f $(OBJ)
test : timecard
	@./timecard 01-Jan-2008 31-Dec-2008 timecard.txt
valgrind : timecard
	@valgrind ./timecard 01-Jan-2008 31-Dec-2008 timecard.txt
sparse :
	@sparse $(SRC)
