CC = gcc
MAIN_FILE = main
LOOKUP5_FILE = lookup5
LOOKUP9_FILE = lookup9
UDP_SERVER_FILE = udp_server
UDP_CILENT_FILE = udp_client

all:run

run:
	$(CC) -c $(MAIN_FILE).c $(LOOKUP5_FILE).c $(LOOKUP9_FILE).c $(UDP_SERVER_FILE).c
	$(CC) -o $(UDP_CILENT_FILE) $(MAIN_FILE).o $(LOOKUP9_FILE).o
	$(CC) -o $(UDP_SERVER_FILE) $(UDP_SERVER_FILE).o $(LOOKUP5_FILE).o
	clear

	# instruction for HW11 part3
	# 	./udp_server <dictionary>
	# 	./udp_client <socket_name>

clean:
	rm -f $(MAIN_FILE).o $(LOOKUP5_FILE).o $(LOOKUP9_FILE).o $(UDP_SERVER_FILE).o
	rm -f $(UDP_CILENT_FILE) $(UDP_SERVER_FILE)