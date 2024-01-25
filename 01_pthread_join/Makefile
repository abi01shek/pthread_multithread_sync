objects = main 

CFLAGS=-g -Wall -pthread

all: $(objects)

$(objects): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm $(objects)
