CC = gcc
SERVER_FILE = server
CLIENT_FILE = client

all:run

run:
	$(CC) -c $(SERVER_FILE).c $(CLIENT_FILE).c
	$(CC) -o $(SERVER_FILE) $(SERVER_FILE).o
	$(CC) -o $(CLIENT_FILE) $(CLIENT_FILE).o
	clear

	# instruction for HW9
	# 1.
	# 	./server &
	# 	./client <file>

clean:
	rm -f $(SERVER_FILE) $(SERVER_FILE).o
	rm -f $(CLIENT_FILE) $(CLIENT_FILE).o
