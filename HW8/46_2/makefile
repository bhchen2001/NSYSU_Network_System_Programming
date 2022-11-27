CC = gcc
CLIENT_FILE = client
SERVER_FILE = server

all:run

run:
	$(CC) -c $(CLIENT_FILE).c $(SERVER_FILE).c
	$(CC) -o $(CLIENT_FILE) $(CLIENT_FILE).o
	$(CC) -o $(SERVER_FILE) $(SERVER_FILE).o

	clear
	# instruction for HW8 46_2
	# 	./server
	# 	./client <seq-len>

clean:
	rm -f $(CLIENT_FILE) $(CLIENT_FILE).o
	rm -f $(SERVER_FILE) $(SERVER_FILE).o
