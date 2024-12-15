laba_3: main.o realise.o
	gcc main.o realise.o -o laba_3
main.o: main.c
	gcc -c main.c -o main.o
realise.o: realise.c
	gcc -c realise.c -o realise.o
