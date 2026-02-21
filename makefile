all:
	gcc -o web-server.out ./src/main.c ./src/server.c ./src/http.c ./src/file.c

clean:
	rm -f server
