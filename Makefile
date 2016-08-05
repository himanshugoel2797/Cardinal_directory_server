TARGET=file_server.elf

CC=x86_64-elf-cardinal-gcc
CFLAGS=

OBJS=main.o

all:$(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

clean:
	rm -rf $(OBJS)
	rm -rf $(TARGET)