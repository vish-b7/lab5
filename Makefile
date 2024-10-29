all: server
 
server: number-server.c http-server.c
	gcc -std=c11 -Wall -Wno-unused-variable -fsanitize=address -g number-server.c http-server.c -o server

clean:
	rm -f server
