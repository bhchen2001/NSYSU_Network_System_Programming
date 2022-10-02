CC = gcc
SHELL_FILE = shell
PARSE_FILE = parse
BUILTIN_FILE = builtin
RUN_COMMAND_FILE = run_command
IS_BACKGROUND = is_background


all:run
run:
	$(CC) -c $(SHELL_FILE).c $(PARSE_FILE).c $(BUILTIN_FILE).c $(RUN_COMMAND_FILE).c $(IS_BACKGROUND).c
	$(CC) -o $(SHELL_FILE) $(SHELL_FILE).o $(PARSE_FILE).o $(BUILTIN_FILE).o $(RUN_COMMAND_FILE).o $(IS_BACKGROUND).o
	clear
	#
	# instruction of hw2
	# 1. tail
	# 		./shell

clean:
	rm -f $(SHELL_FILE) $(SHELL_FILE).o $(PARSE_FILE).o $(BUILTIN_FILE).o $(RUN_COMMAND_FILE).o $(IS_BACKGROUND).o
