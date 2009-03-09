OBJ=main.o efunc.o utils.o
SRC=main.c efunc.c utils.c

CFLAGS=-O2 -g -Wall -Wwrite-strings

timecard : $(OBJ)
	gcc -o $@ $(OBJ)
main.o : efunc.h utils.h
efunc.o : efunc.h utils.h
utils.o : efunc.h utils.h
clean :
	rm -f $(OBJ)
test : timecard
	@./timecard 2009-02-16 2009-02-26 timecard.txt
valgrind : timecard
	@valgrind ./timecard 2009-02-23 2009-02-26 timecard.txt
sparse :
	@sparse $(SRC)
