CFLAGS=-I./ -Wall -Wstrict-prototypes -Werror -g
CC=gcc

all: test 

test: fifo.o fifo_test.o 
	$(CC) -o $@ $? -pthread
	
%.o:%.c
	$(CC) $(CFLAGS) -c $? -o $@

clean:
	rm -f *.o *~
