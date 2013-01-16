CC=gcc
LIBS=-lreadline -lpthread
EXEC=myshell
all:$(EXEC)
CCFLAGS=-g -Wall

$(EXEC): main.o cmd.o shell_fct.o 
	gcc $(CCFLAGS) -o  $(EXEC) main.o cmd.o shell_fct.o $(LIBS)

cmd.o: cmd.c
	$(CC)  $(CCFLAGS) -o cmd.o -c cmd.c

shell_fct.o: shell_fct.c
	$(CC)  $(CCFLAGS) -o shell_fct.o -c shell_fct.c
 
main.o: main.c
	$(CC)  $(CCFLAGS) -o main.o -c main.c
 
clean:
	rm -vf *.o