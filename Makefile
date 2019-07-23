CFLAGS=-Wall	-g
src = $(wildcard *.c)
obj = $(src:.c=.o)

snake: $(obj)
	gcc	-o $@ $^	
clean:
	rm -f $(object)	*o	snake
