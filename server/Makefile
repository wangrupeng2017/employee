

CC=gcc
Libs=sqlite3
Objs=$(wildcard store/*.c) $(wildcard network/*.c) $(wildcard business/*.c) $(wildcard iopoll/*.c)
Includes=-I./store -I./network -I./business -I./iopoll -I../common/ 

main:$(Objs) main.c
	$(CC) main.c $(Objs) -o $@ -l $(Libs) $(Includes)


clean:
	rm main
