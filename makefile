all:
	gcc -o web-server.out ./src/main.c ./src/server.c ./src/http.c ./src/file.c ./src/server_config.c ./src/cJSON.c

clean:
	rm -f server
