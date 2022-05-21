CC=g++
CPPFLAGS=-Wall -Wextra -std=c++11

client: client.cpp requests.c helpers.cpp buffer.c io.cpp client_functions.cpp
	$(CC) -o client client.cpp requests.c helpers.cpp buffer.c io.cpp client_functions.cpp $(CPPFLAGS)

run: client
	./client

clean:
	rm -f *.o client
