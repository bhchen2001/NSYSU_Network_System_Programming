CC = gcc
HOSTINFO = hostinfo
MYDATE = mydate
PRINTDIR = printdir
MYCAT = mycat
PIPE_LS = pipe_ls

all:run

run:
	$(CC) -c $(HOSTINFO).c $(MYDATE).c $(PRINTDIR).c $(MYCAT).c $(PIPE_LS).c
	$(CC) -o $(HOSTINFO) $(HOSTINFO).o
	$(CC) -o $(MYDATE) $(MYDATE).o
	$(CC) -o $(PRINTDIR) $(PRINTDIR).o
	$(CC) -o $(MYCAT) $(MYCAT).o
	$(CC) -o $(PIPE_LS) $(PIPE_LS).o

	clear

	# instruction of HW5_part1
	# ./hostinfo
	# ./mydate
	# ./printdir
	# ./mycat <filename>
	# ./pipe_ls

clean:
	rm -f $(HOSTINFO).o $(HOSTINFO)
	rm -f $(MYDATE).o $(MYDATE)
	rm -f $(PRINTDIR).o $(PRINTDIR)
	rm -f $(MYCAT).o $(MYCAT)
	rm -f $(PIPE_LS).o $(PIPE_LS)
