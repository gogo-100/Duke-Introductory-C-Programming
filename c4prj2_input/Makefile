CFLAGS=-Wall -Werror -std=gnu99 -pedantic -ggdb3
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
PROGRAM=test-c4prj2

$(PROGRAM): $(OBJS)
	gcc $(CFLAGS) -o $@ $(OBJS)

%.o: %.c 
	gcc -c $(CFLAGS) $<

clean:
	rm -f $(OBJS) $(PROGRAM) *~

deck.o: deck.h
eval.o: eval.h
cards.o: cards.h
future.o: future.h
test.o: deck.h
test.o: eval.h
test.o: cards.h
test.o: future.h
test.o: input.c
