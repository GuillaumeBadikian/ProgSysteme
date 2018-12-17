cc=gcc
flag=-Wall

main: main2 init.o
	$(cc) $(flag) -o exam init.o 

main2: journaliste.o
	$(cc) $(flag) -o journaliste journaliste.o

journaliste.o : journaliste.c journaliste.h
	$(cc) $(flag) -c journaliste.c

init.o: init.c init.h
	$(cc) $(flag) -c init.c

clean:
	rm -rf *.o exam journaliste
