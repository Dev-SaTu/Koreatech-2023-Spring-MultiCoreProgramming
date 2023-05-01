CC = gcc
CFLAGS = -c -g
LDFLAGS =  
OBJECTS = main.o

run: all
	program

all: program

program : $(OBJECTS)
	$(CC)  $(LDFLAGS) -o program $(OBJECTS)

main.o : main.c
	$(CC) $(CFLAGS) main.c 

clean:
	rm -f $(SRCS:.c=.o)