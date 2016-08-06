TARGET=file_server.elf

CC=x86_64-elf-cardinal-gcc
CFLAGS= -std=c11 -D_GNU_SOURCE

OBJS=main.o open.o close.o read.o write.o list.o mount_db.o interface_provider.o

all:$(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

clean:
	rm -rf $(OBJS)
	rm -rf $(TARGET)