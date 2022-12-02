CC = gcc
MAIN_FILE = main
LOOKUP5_FILE = lookup5
LOOKUP8_FILE = lookup8
ISOCK_SERVER_FILE = isock_server
ISOCK_CILENT_FILE = isock_client

all:run

run:
	$(CC) -c $(MAIN_FILE).c $(LOOKUP5_FILE).c $(LOOKUP8_FILE).c $(ISOCK_SERVER_FILE).c
	$(CC) -o $(ISOCK_CILENT_FILE) $(MAIN_FILE).o $(LOOKUP8_FILE).o
	$(CC) -o $(ISOCK_SERVER_FILE) $(ISOCK_SERVER_FILE).o $(LOOKUP5_FILE).o
	clear

	# instruction for HW11 part2
	# 	./isock_server <dictionary>
	# 	./isock_client <socket_name>

clean:
	rm -f $(MAIN_FILE).o $(LOOKUP5_FILE).o $(LOOKUP8_FILE).o $(ISOCK_SERVER_FILE).o
	rm -f $(ISOCK_CILENT_FILE) $(ISOCK_SERVER_FILE)