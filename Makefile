cc=gcc
flag=-Wall

main:  archiviste journaliste init.o 
	$(cc) $(flag) -o exam init.o


archiviste: archiviste.c archiviste.h
	$(cc) $(flag) -o archiviste archiviste.c
  
journaliste : journaliste.c journaliste.h
	$(cc) $(flag) -o journaliste journaliste.c

init.o: init.c init.h
	$(cc) $(flag) -c init.c


clean:
	rm -rf *.o exam journaliste
