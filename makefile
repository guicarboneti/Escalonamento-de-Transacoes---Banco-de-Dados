CC     = gcc
CFLAGS = -Wall -g

PROG = escalona
OBJS = seriabilidade.o visao.o

.PHONY: clean purge all

all: $(PROG)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

$(PROG) : % :  $(OBJS) %.o
	$(CC) -o $@ $^

clean:
	@rm -f *.o

purge:   
	@rm -f *.o $(PROG)