cc=gcc
flag=-Wall

main: init.o
	$(cc) $(flag) -o exam init.o -ly -ll


init.o: init.c init.h
	$(cc) $(flag) -c init.c

clean:
	rm -rf *.o exam
