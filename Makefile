cc=gcc
flag=-Wall

main: init.o
	$(cc) $(flag) -o exam init.o 


init.o: init.c init.h
	$(cc) $(flag) -c init.c

clean:
	rm -rf *.o exam
