OBJECTS := huffman.o msg.o cl_parse.o common.o gdemo.o opts.o dump.o parser.o helper.o stats.o print.o

CC := gcc
CFLAGS := -Wall# -g -pg -DDEBUG
LFLAGS :=

OUTPUT := gdemo

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.c: %.h

clean:
	rm -f *.o $(OUTPUT)
