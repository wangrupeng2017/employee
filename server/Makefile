

CC=gcc
Libs=sqlite3
Objs=$(wildcard sqlite3/*.c) \
	 $(wildcard socket/*.c) $(wildcard business/*.c)
Includes=-I./sqlite3 -I./socket -I./business -I../common/ 

main:$(Objs) main.c
	$(CC) main.c $(Objs) -o $@ -l $(Libs) $(Includes)

client:$(Objs) client.c
	$(CC) client.c $(Objs) -o $@ -l $(Libs) $(Includes)

clean:
	rm main client
