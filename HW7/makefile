CC = gcc
SIG_SPEED_FILE = sig_speed_sigsuspend
22_3_FILE = 22_3
22_4_FILE = 22_4

all:run

run:
	CC -c $(22_3_FILE).c $(22_4_FILE).c
	CC -o $(22_3_FILE) $(22_3_FILE).o
	CC -o $(22_4_FILE) $(22_4_FILE).o

	clear
	# instruction of HW6
	# 1. 
	#	./22_3 <sig_num>
	# 2.
	# 	./22_4 <func>

clean:
	rm -f $(22_3_FILE) $(22_3_FILE).o
	rm -f $(22_4_FILE) $(22_4_FILE).o
