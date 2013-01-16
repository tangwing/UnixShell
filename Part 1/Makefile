CC=gcc
#-lreadline
LIBS= -lreadline -lpthread
EXEC=myshell
all:$(EXEC)
CCFLAGS=-g -Wall

$(EXEC): main.o cmd_membres.o cmd_args.o cmd_redirect.o  shell_fct.o 
	gcc $(CCFLAGS) -o  $(EXEC) main.o cmd_membres.o cmd_args.o cmd_redirect.o  shell_fct.o $(LIBS)

cmd_membres.o: cmd_membres.c 
	$(CC)  $(CCFLAGS) -o cmd_membres.o -c cmd_membres.c

cmd_args.o: cmd_args.c
	$(CC)  $(CCFLAGS) -o cmd_args.o -c cmd_args.c

cmd_redirect.o: cmd_redirect.c 
	$(CC)  $(CCFLAGS) -o cmd_redirect.o -c cmd_redirect.c 

shell_fct.o: shell_fct.c
	$(CC)  $(CCFLAGS) -o shell_fct.o -c shell_fct.c
 
main.o: main.c
	$(CC)  $(CCFLAGS) -o main.o -c main.c
 
clean:
	rm -vf *.o
