CC = gcc
MAIN_FILE = main
LOOKUP5_FILE = lookup5
LOOKUP7_FILE = lookup7
USOCK_SERVER_FILE = usock_server
USOCK_CILENT_FILE = usock_client
TUNNEL_FILE = TUNNEL

all:run

run:
	$(CC) -c $(MAIN_FILE).c $(LOOKUP5_FILE).c $(LOOKUP7_FILE).c $(USOCK_SERVER_FILE).c
	$(CC) -o $(USOCK_CILENT_FILE) $(MAIN_FILE).o $(LOOKUP7_FILE).o
	$(CC) -o $(USOCK_SERVER_FILE) $(USOCK_SERVER_FILE).o $(LOOKUP5_FILE).o
	rm -f $(TUNNEL_FILE) 
	clear

	# instruction for HW11 part1
	# 	./usock_server <dictionary> <socket_name>
	# 	./usock_client <socket_name>

clean:
	rm -f $(MAIN_FILE).o $(LOOKUP5_FILE).o $(LOOKUP7_FILE).o $(USOCK_SERVER_FILE).o
	rm -f $(USOCK_CILENT_FILE) $(USOCK_SERVER_FILE)  
	rm -f $(TUNNEL_FILE) 