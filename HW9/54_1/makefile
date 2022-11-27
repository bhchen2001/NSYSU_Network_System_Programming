CC = gcc
WRITER_FILE = writer
READER_FILE = reader
OUTPUT_FILE = out

all:run

run:
	$(CC) -c $(WRITER_FILE).c $(READER_FILE).c
	$(CC) -o $(WRITER_FILE) $(WRITER_FILE).o
	$(CC) -o $(READER_FILE) $(READER_FILE).o
	clear

	# instruction for HW9
	# 1.
	# 	./writer < <file> &
	# 	./reader > out.txt

clean:
	rm -f $(WRITER_FILE) $(WRITER_FILE).o
	rm -f $(READER_FILE) $(READER_FILE).o
	rm -f $(OUTPUT_FILE).txt
