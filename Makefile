TARGET=file_server.elf

CC=x86_64-elf-cardinal-gcc
CFLAGS= -std=c11

OBJS=main.o open.o close.o read.o write.o list.o

all:$(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

clean:
	rm -rf $(OBJS)
	rm -rf $(TARGET)