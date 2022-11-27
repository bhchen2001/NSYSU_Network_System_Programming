CC = gcc
MISBEHAVE_CLIENT_FILE = fifo_seqnum_client_misbehave
CLIENT_FILE = fifo_seqnum_client
SERVER_FILE = fifo_seqnum_server
GET_NUM_FILE = get_num
ERROR_FUNCTION_FILE = error_functions

all:run

run:
	$(CC) -c $(CLIENT_FILE).c $(MISBEHAVE_CLIENT_FILE).c $(SERVER_FILE).c $(GET_NUM_FILE).c $(ERROR_FUNCTION_FILE).c
	$(CC) -o $(CLIENT_FILE) $(CLIENT_FILE).o $(GET_NUM_FILE).o $(ERROR_FUNCTION_FILE).o
	$(CC) -o $(SERVER_FILE) $(SERVER_FILE).o $(GET_NUM_FILE).o $(ERROR_FUNCTION_FILE).o
	$(CC) -o $(MISBEHAVE_CLIENT_FILE) $(MISBEHAVE_CLIENT_FILE).o $(GET_NUM_FILE).o $(ERROR_FUNCTION_FILE).o


	clear
	# instruction for HW8
	# 1.
	# 	./fifo_seqnum_server
	# 	./fifo_seqnum_client
	# 	./ fifo_seqnum_client_misbehave

clean:
	rm -f $(CLIENT_FILE) $(CLIENT_FILE).o
	rm -f $(SERVER_FILE) $(SERVER_FILE).o
	rm -f $(MISBEHAVE_CLIENT_FILE) $(MISBEHAVE_CLIENT_FILE).o
	rm -f $(GET_NUM_FILE).o $(ERROR_FUNCTION_FILE).o
	rm -f seqnum_sv seqnum_cl*
