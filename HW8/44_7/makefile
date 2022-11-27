CC = gcc
44_7_FILE = 44_7
FIFO_FILE = fifo_file_44_7

all:run

run:
	$(CC) -c $(44_7_FILE).c
	$(CC) -o $(44_7_FILE) $(44_7_FILE).o

	clear
	# instruction of HW8 - 44_7
	# 1.
	# 	./44_7

clean:
	rm -f $(FIFO_FILE)
	rm -f $(44_7_FILE) $(44_7_FILE).o
