OBJ=main.o efunc.o
SRC=main.c efunc.c

CFLAGS=-g -Wall

timecard : main.o efunc.o
	gcc -o $@ main.o efunc.o
main.o : efunc.h
efunc.o : efunc.h

clean :
	rm -f $(OBJ)
test : timecard
	@./timecard 2009-01-01 2009-01-07 timecard.txt
valgrind : timecard
	@valgrind ./timecard 2009-01-01 2009-01-07 timecard.txt
sparse :
	@sparse $(SRC)
