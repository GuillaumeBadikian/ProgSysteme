cc=gcc
flag=-Wall

main:  archiviste init.o 
	$(cc) $(flag) -o exam init.o




init.o: init.c init.h
	$(cc) $(flag) -c init.c

archiviste: archiviste.c archiviste.h
	$(cc) $(flag) -o archiviste archiviste.c

clean:
	rm -rf *.o exam
