all:
	gcc -o web-server.out ./src/main.c ./src/server.c ./src/http.c

clean:
	rm -f server
